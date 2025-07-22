#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <boost/asio.hpp>
#include "Maths/Vec2.hpp"
#include "Core/EntityManager.hpp"
#include "Core/ComponentManager.hpp"
#include "Core/SystemManager.hpp"
#include "Core/Coordinator.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/InitialPositionComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/PlayerComponent.hpp"

#include "Systems/MovementSystem.hpp"
#include "Systems/ProjectileSystem.hpp"
#include "Systems/CollisionSystem.hpp"


#include "Mutex.hpp"
#include "config.hpp"
#include "../shared/Config.hpp"

std::ostream& operator<<(std::ostream& os, const PositionComponent& posComponent);
Coordinator gCoordinator;

std::map<udp::endpoint, Entity> endpointToEntityMap;
bool didKill = true;

void levelOne(struct GameStruct *gameStruct, Coordinator& gCoordinator);
void levelTwo(struct GameStruct *gameStruct, Coordinator& gCoordinator);

void callFunctionPeriodically(struct GameStruct* gameStruct) {
    const int targetFPS = 60;  // Desired frames per second
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    
    while (true) {
        auto frameStartTime = std::chrono::steady_clock::now();

        // Call your functions here
        gameStruct->projectileSystem->ProjectileRoutine(gameStruct);
        gameStruct->movementSystem->UpdateEnemies(gameStruct);
        gameStruct->collisionSystem->Update(gameStruct);

        {
            std::unique_lock<std::mutex> levelOneLock(levelOneMutex);
            std::unique_lock<std::mutex> levelTwoLock(levelTwoMutex);
            if (!checkIfLevel(gameStruct)) {
                if (!didKill) {
                    didKill = true;
                    killEveryEntities(gameStruct);
                }
            } else {
                if (!checkIfAlive()) {
                    gameStruct->startLevelOne = false;
                    gameStruct->startLevelTwo = false;
                    std::cout << "Players died - Level ends" << std::endl;
                }
            }
        }

        auto frameEndTime = std::chrono::steady_clock::now();
        auto frameDurationElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEndTime - frameStartTime);

        // Calculate how much time to sleep to achieve the desired frame rate
        auto sleepTime = frameDuration - frameDurationElapsed;

        std::this_thread::sleep_for(sleepTime);
    }
}

int main() {
    // Register Components and Systems
    GameStruct* gameStruct = new GameStruct(gCoordinator);

    try {
        boost::asio::io_context io_context;

        // Create a UDP socket
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 9876));
        std::thread sendStructThread(sendStructPeriodically, std::ref(socket), std::ref(gameStruct));
        std::thread periodicCaller(callFunctionPeriodically, std::ref(gameStruct));
        std::thread levelOneThread;
        std::thread levelTwoThread;

        while (true) {
            char data[1024] = {0}; // Initialize the buffer with zeros
            udp::endpoint sender_endpoint;
            boost::system::error_code error;

            // Receive data from clients
            size_t length = socket.receive_from(boost::asio::buffer(data), sender_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size) {
                std::cerr << "Error: " << error.message() << std::endl;
            } else {
                if (length == sizeof(uint16_t)) {
                    // Convert the received data to an integer value.
                    uint16_t receivedValue;
                    std::memcpy(&receivedValue, data, sizeof(receivedValue));

                    std::cout << "Received (smallint): " << receivedValue << " from " << sender_endpoint << std::endl;

                    // Check which range the received value falls into.
                    if (receivedValue == 1) {
                        // Move player to the left
                        gameStruct->movementSystem->Move(Vec2(-5.0f, 0.0f), endpointToEntityMap[sender_endpoint]);
                    } else if (receivedValue == 2) {
                        // Move player to the right
                        gameStruct->movementSystem->Move(Vec2(5.0f, 0.0f), endpointToEntityMap[sender_endpoint]);
                    } else if (receivedValue == 3) {
                        // Move player up
                        gameStruct->movementSystem->Move(Vec2(0.0f, -5.0f), endpointToEntityMap[sender_endpoint]);
                    } else if (receivedValue == 4) {
                        // Move player down
                        gameStruct->movementSystem->Move(Vec2(0.0f, 5.0f), endpointToEntityMap[sender_endpoint]);
                    } else if (receivedValue == 5) {
                        // Player take damage (DEBUG)
                        gameStruct->damageSystem->TakeDamage(1, endpointToEntityMap[sender_endpoint]);
                    } else if (receivedValue == 6) {
                        // Player shoot projectile
                        createProjectilesPlayerSafely(gameStruct, sender_endpoint);
                    } else if (receivedValue == 21) {
                        {
                            std::unique_lock<std::mutex> levelOneLock(levelOneMutex);
                            std::unique_lock<std::mutex> levelTwoLock(levelTwoMutex);
                            if (!gameStruct->startLevelOne) {
                                std::cout << "Level1 start" << std::endl;
                                // Game start and levelOne starts
                                if (!checkIfLevel(gameStruct)) {
                                    revivePlayers(gameStruct);
                                    gameStruct->startLevelOne = true;
                                    didKill = false;
                                    // Start the "levelOne" thread.
                                    levelOneThread = std::thread(levelOne, gameStruct, std::ref(gCoordinator));

                                    // Optionally, you can detach the thread to let it run independently.
                                    levelOneThread.detach();
                                }
                            }
                        }
                    } else if (receivedValue == 22) {
                        {
                            std::unique_lock<std::mutex> levelOneLock(levelOneMutex);
                            std::unique_lock<std::mutex> levelTwoLock(levelTwoMutex);
                            if (!gameStruct->startLevelTwo) {
                                std::cout << "Level2 start" << std::endl;
                                // Game start and levelTwo starts
                                if (!checkIfLevel(gameStruct)) {
                                    revivePlayers(gameStruct);
                                    gameStruct->startLevelTwo = true;
                                    didKill = false;
                                    // Start the "levelTwo" thread.
                                    levelTwoThread = std::thread(levelTwo, gameStruct, std::ref(gCoordinator));

                                    // Optionally, you can detach the thread to let it run independently.
                                    levelTwoThread.detach();
                                }
                            }
                        }
                    } else if (receivedValue == 42) {
                        std::cout << "Disconnect Client" << std::endl;
                        std::lock_guard<std::mutex> lock(clientMutex);

                        // Find the client's endpoint in playerVector
                        auto it = endpointToEntityMap.find(sender_endpoint);
                        if (it != endpointToEntityMap.end()) {
                            // Remove the client's Entity
                            Entity foundEntity = it->second;
                            gameStruct->damageSystem->TakeDamage(3, foundEntity);
                            // gCoordinator.DestroyEntity(foundEntity);  // Destroy the Entity
                            endpointToEntityMap.erase(it);
                        }
                    }
                } else {
                    // When received data is not a small int (it is probably just the name)
                    std::cout << "Received (str): " << data << " from " << sender_endpoint << std::endl;
                    std::string message(data, length);

                    // Check if the client is already logged in
                    if (endpointToEntityMap.find(sender_endpoint) == endpointToEntityMap.end()) {
                        // Limit the number of clients to 4
                        if (endpointToEntityMap.size() < 4) {
                            createPlayer(gameStruct, gCoordinator, message, sender_endpoint);
                            std::cout << "Client is now logged in from " << sender_endpoint << std::endl;
                        } else {
                            std::cout << "Maximum number of clients reached. Connection refused for " << sender_endpoint << std::endl;
                            continue; // Skip further processing for this client (max reached)
                        }
                    }
                }

                // Check if client disconnects
                if (error == boost::asio::error::connection_reset || error == boost::asio::error::connection_aborted) {
                    std::lock_guard<std::mutex> lock(clientMutex);

                    // Find the client's endpoint in playerVector
                    auto it = endpointToEntityMap.find(sender_endpoint);

                    if (it != endpointToEntityMap.end()) {
                        // Remove the client's Entity
                        Entity foundEntity = it->second;
                        gCoordinator.DestroyEntity(foundEntity);  // Destroy the Entity
                        endpointToEntityMap.erase(it);
                    }
                }
            }
        }

        sendStructThread.join();
        periodicCaller.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
