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
extern Coordinator gCoordinator;
extern std::map<udp::endpoint, Entity> endpointToEntityMap;

struct PosAndSprite {
    int posX;
    int posY;
    int spriteCode;
    int id;
    bool isAlive;
};

bool sendEntityData(const Entity entity, udp::socket& socket)
{
    PosAndSprite data;

    // Modify these lines to get the X, Y, and sprite code from the entity
    data.posX = static_cast<int>(gCoordinator.GetComponent<PositionComponent>(entity).position.x);
    data.posY = static_cast<int>(gCoordinator.GetComponent<PositionComponent>(entity).position.y);
    data.spriteCode = gCoordinator.GetComponent<SpriteComponent>(entity).texturePath;
    data.id = gCoordinator.GetComponent<SpriteComponent>(entity).id;
    data.isAlive = gCoordinator.GetComponent<SpriteComponent>(entity).isAlive;


    std::vector<char> buffer(sizeof(data));
    std::memcpy(buffer.data(), &data, sizeof(data));

    for (const auto& clientEntry : endpointToEntityMap) {
        const udp::endpoint& clientEndpoint = clientEntry.first;
        socket.send_to(boost::asio::buffer(buffer), clientEndpoint);
    }

    return(data.isAlive);
}

void sendStructPeriodically(udp::socket& socket, struct GameStruct *gameStruct) {
    const int targetFPS = 60;  // Desired frames per second
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);

    while (true) {
        auto frameStartTime = std::chrono::steady_clock::now();
        {
            std::unique_lock<std::mutex> playerLock(clientMutex);
            for (const auto& entityEntry : endpointToEntityMap) {
                const Entity entity = entityEntry.second;
                PosAndSprite data;

                // Modify these lines to get the X, Y, and sprite code from the entity
                data.posX = static_cast<int>(gCoordinator.GetComponent<PositionComponent>(entity).position.x);
                data.posY = static_cast<int>(gCoordinator.GetComponent<PositionComponent>(entity).position.y);
                data.spriteCode = gCoordinator.GetComponent<SpriteComponent>(entity).texturePath;
                data.id = gCoordinator.GetComponent<SpriteComponent>(entity).id;
                data.isAlive = gCoordinator.GetComponent<SpriteComponent>(entity).isAlive;

                std::vector<char> buffer(sizeof(data));
                std::memcpy(buffer.data(), &data, sizeof(data));

                for (const auto& clientEntry : endpointToEntityMap) {
                    const udp::endpoint& clientEndpoint = clientEntry.first;
                    socket.send_to(boost::asio::buffer(buffer), clientEndpoint);
                }
            }
        }
        {
            std::unique_lock<std::mutex> mosquitoLock(mosquitoMutex);
            for (auto it = gameStruct->enemiesMosquitoVector.begin(); it != gameStruct->enemiesMosquitoVector.end();) {
                const Entity entity = *it;

                if (!sendEntityData(entity, socket)) {
                    std::cout << "Destroying entity directly - Mosquito" << std::endl;
                    gCoordinator.DestroyEntity(entity);
                    it = gameStruct->enemiesMosquitoVector.erase(it); // Remove the entity from the vector
                } else {
                    ++it;
                }
            }
        }
        {
            std::unique_lock<std::mutex> planeLock(planeMutex);
            for (auto it = gameStruct->enemiesPlaneVector.begin(); it != gameStruct->enemiesPlaneVector.end();) {
                const Entity entity = *it;

                if (!sendEntityData(entity, socket)) {
                    std::cout << "Destroying entity directly - Plane" << std::endl;
                    gCoordinator.DestroyEntity(entity);
                    it = gameStruct->enemiesPlaneVector.erase(it); // Remove the entity from the vector
                } else {
                    ++it;
                }
            }
        }
        {
            std::unique_lock<std::mutex> alienBossLock(alienBossMutex);
            for (auto it = gameStruct->enemiesAlienBossVector.begin(); it != gameStruct->enemiesAlienBossVector.end();) {
                const Entity entity = *it;

                if (!sendEntityData(entity, socket)) {
                    std::cout << "Destroying entity directly - AlienBoss" << std::endl;
                    gCoordinator.DestroyEntity(entity);
                    it = gameStruct->enemiesAlienBossVector.erase(it); // Remove the entity from the vector
                } else {
                    ++it;
                }
            }
        }
        {
            std::unique_lock<std::mutex> planeProjectileLock(planeProjectileMutex);
            for (auto it = gameStruct->projectilesPlaneVector.begin(); it != gameStruct->projectilesPlaneVector.end();) {
                const Entity entity = *it;

                if (!sendEntityData(entity, socket)) {
                    std::cout << "Destroying entity directly - PlaneProjectile" << std::endl;
                    gCoordinator.DestroyEntity(entity);
                    it = gameStruct->projectilesPlaneVector.erase(it); // Remove the entity from the vector
                } else {
                    ++it;
                }
            }
        }
        {
            std::unique_lock<std::mutex> straightProjectileLock(straightProjectileMutex);
            for (auto it = gameStruct->projectilesStraightVector.begin(); it != gameStruct->projectilesStraightVector.end();) {
                const Entity entity = *it;

                if (!sendEntityData(entity, socket)) {
                    std::cout << "Destroying entity directly - StraightProjectile" << std::endl;
                    gCoordinator.DestroyEntity(entity);
                    it = gameStruct->projectilesStraightVector.erase(it); // Remove the entity from the vector
                } else {
                    ++it;
                }
            }
        }
        {
            std::unique_lock<std::mutex> projectileLock(projectilesPlayerMutex);
            for (auto it = gameStruct->projectilesPlayerVector.begin(); it != gameStruct->projectilesPlayerVector.end();) {
                const Entity entity = *it;

                if (!sendEntityData(entity, socket)) {
                    std::cout << "Destroying entity directly - Projectile" << std::endl;
                    gCoordinator.DestroyEntity(entity);
                    it = gameStruct->projectilesPlayerVector.erase(it); // Remove the entity from the vector
                } else {
                    ++it;
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