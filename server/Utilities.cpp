#include "Components/PositionComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Maths/Vec2.hpp"
#include "Core/Coordinator.hpp"

#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#include "Mutex.hpp"
#include "config.hpp"
#include "../shared/Config.hpp"

extern Coordinator gCoordinator;

bool checkIfLevel(struct GameStruct *gameStruct) {
    if (gameStruct->startLevelOne == true)
        return true;
    if (gameStruct->startLevelTwo == true)
        return true;
    
    return false;
}

bool checkIfAlive() {
    std::unique_lock<std::mutex> playerLock(clientMutex);

    for (const auto& entityEntry : endpointToEntityMap) {
        const Entity entity = entityEntry.second;
        bool alive = gCoordinator.GetComponent<SpriteComponent>(entity).isAlive;

        if (alive) {
            // If at least one player is alive, return true immediately
            return true;
        }
    }

    // If the loop completes without finding any alive players, return false
    return false;
}

std::ostream& operator<<(std::ostream& os, const PositionComponent& posComponent) {
    os << "Position: (" << posComponent.position.x << ", " << posComponent.position.y << ")";
    return os;
}

sf::Color getRandomColor() {
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    return sf::Color(r, g, b);
}

Vec2 getRandomPos() {
    float x = rand() % 700;
    float y = rand() % 500;
    return Vec2(x, y);
}

void createMosquitoSafely(struct GameStruct *gameStruct) {
    // Lock the mutex to ensure exclusive access to the mosquito vector.
    std::lock_guard<std::mutex> lock(mosquitoMutex);

    createMosquito(gameStruct, gCoordinator);
}

void createPlaneSafely(struct GameStruct *gameStruct) {
    // Lock the mutex to ensure exclusive access to the plane vector.
    std::lock_guard<std::mutex> lock(planeMutex);

    createPlane(gameStruct, gCoordinator);
}

void createAlienBossSafely(struct GameStruct *gameStruct) {
    // Lock the mutex to ensure exclusive access to the alienBoss vector.
    std::lock_guard<std::mutex> lock(alienBossMutex);

    createAlienBoss(gameStruct, gCoordinator);
}

void createProjectilesPlayerSafely(struct GameStruct *gameStruct, udp::endpoint sender_endpoint) {
    // Lock the mutex to ensure exclusive access to the player projectile vector.
    std::lock_guard<std::mutex> lock(projectilesPlayerMutex);

    createProjectilesPlayer(gameStruct, gCoordinator, sender_endpoint);
}

bool revivePlayers(struct GameStruct *gameStruct) {
    std::unique_lock<std::mutex> playerLock(clientMutex);
    if (endpointToEntityMap.empty())
        return false;
    
    for (const auto& entityEntry : endpointToEntityMap) {
        const Entity entity = entityEntry.second;
        gameStruct->movementSystem->Set(Vec2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2), entity);
        gameStruct->damageSystem->HealDamage(3, entity);
    }

    return true;
}

void killEveryEntities(struct GameStruct *gameStruct) {
    {
        std::unique_lock<std::mutex> mosquitoLock(mosquitoMutex);
        for (auto it = gameStruct->enemiesMosquitoVector.begin(); it != gameStruct->enemiesMosquitoVector.end();) {
            Entity entity = *it;

            gameStruct->damageSystem->TakeDamage(100, entity);
            ++it;
        }
    }
    {
        std::unique_lock<std::mutex> planeLock(planeMutex);
        for (auto it = gameStruct->enemiesPlaneVector.begin(); it != gameStruct->enemiesPlaneVector.end();) {
            Entity entity = *it;

            gameStruct->damageSystem->TakeDamage(100, entity);
            ++it;
        }
    }
    {
        std::unique_lock<std::mutex> alienBossLock(alienBossMutex);
        for (auto it = gameStruct->enemiesAlienBossVector.begin(); it != gameStruct->enemiesAlienBossVector.end();) {
            Entity entity = *it;

            gameStruct->damageSystem->TakeDamage(100, entity);
            ++it;
        }
    }
    {
        std::unique_lock<std::mutex> planeProjectileLock(planeProjectileMutex);
        for (auto it = gameStruct->projectilesPlaneVector.begin(); it != gameStruct->projectilesPlaneVector.end();) {
            Entity entity = *it;

            gameStruct->damageSystem->TakeDamage(100, entity);
            ++it;
        }
    }
    {
        std::unique_lock<std::mutex> straightProjectileLock(straightProjectileMutex);
        for (auto it = gameStruct->projectilesStraightVector.begin(); it != gameStruct->projectilesStraightVector.end();) {
            Entity entity = *it;

            gameStruct->damageSystem->TakeDamage(100, entity);
            ++it;
        }
    }
    {
        std::unique_lock<std::mutex> projectileLock(projectilesPlayerMutex);
        for (auto it = gameStruct->projectilesPlayerVector.begin(); it != gameStruct->projectilesPlayerVector.end();) {
            Entity entity = *it;

            gameStruct->damageSystem->TakeDamage(100, entity);
            ++it;
        }
    }
}