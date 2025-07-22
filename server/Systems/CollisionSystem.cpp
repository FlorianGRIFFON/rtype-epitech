#include "CollisionSystem.hpp"

#include "../Components/PositionComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Core/Coordinator.hpp"
#include "../Maths/Vec2.hpp"
#include "../config.hpp"
#include "../Mutex.hpp"

#include <vector>

extern Coordinator gCoordinator;
bool CheckBounds(const sf::FloatRect& bounds1, const sf::FloatRect& bounds2);
void CheckCollision(Entity entity1, Entity entity2, struct GameStruct* gameStruct, bool isBoss);
sf::FloatRect CalculateBounds(const PositionComponent& position, const SpriteComponent& sprite);
void HandleCollision(Entity entity1, Entity entity2, struct GameStruct* gameStruct);
void HandleCollisionBoss(Entity entity1, Entity entity2, struct GameStruct* gameStruct);

void CollisionSystem::Update(struct GameStruct* gameStruct)
{
    {
        // Check for collisions between ProjectilesPlayer and Mosquito + Plane
        std::unique_lock<std::mutex> playerProjLock(projectilesPlayerMutex);
        std::unique_lock<std::mutex> mosquitoLock(mosquitoMutex);
        std::unique_lock<std::mutex> planeLock(planeMutex);
        std::unique_lock<std::mutex> alienBossLock(alienBossMutex);
        for (Entity projectilesPlayer : gameStruct->projectilesPlayerVector) {
            for (Entity mosquito : gameStruct->enemiesMosquitoVector) {
                CheckCollision(projectilesPlayer, mosquito, gameStruct, false);
            }

            for (Entity plane : gameStruct->enemiesPlaneVector) {
                CheckCollision(projectilesPlayer, plane, gameStruct, false);
            }

            for (Entity alienBoss : gameStruct->enemiesAlienBossVector) {
                CheckCollision(projectilesPlayer, alienBoss, gameStruct, false);
            }
        }
    }

    {
        // Check for collisions between PlaneProjectile, Plane, Mosquito and Player
        std::unique_lock<std::mutex> playerLock(clientMutex);
        std::unique_lock<std::mutex> planeProjLock(planeProjectileMutex);
        std::unique_lock<std::mutex> mosquitoLock(mosquitoMutex);
        std::unique_lock<std::mutex> planeLock(planeMutex);
        std::unique_lock<std::mutex> alienBossLock(alienBossMutex);
        std::unique_lock<std::mutex> straightProjectileLock(straightProjectileMutex);
        for (auto &entry : endpointToEntityMap) {
            Entity player = entry.second;
            bool isAlive = gCoordinator.GetComponent<SpriteComponent>(player).isAlive;
            if (isAlive) {
                for (Entity planeProjectile : gameStruct->projectilesPlaneVector) {
                    CheckCollision(planeProjectile, player, gameStruct, false);
                }

                for (Entity straightProjectile : gameStruct->projectilesStraightVector) {
                    CheckCollision(straightProjectile, player, gameStruct, false);
                }

                for (Entity mosquito : gameStruct->enemiesMosquitoVector) {
                    CheckCollision(mosquito, player, gameStruct, false);
                }

                for (Entity plane : gameStruct->enemiesPlaneVector) {
                    CheckCollision(plane, player, gameStruct, false);
                }

                for (Entity alienBoss : gameStruct->enemiesAlienBossVector) {
                    CheckCollision(alienBoss, player, gameStruct, false);
                }
            }
        }
    }

    {
        // Check for collisions between Bosses
        std::unique_lock<std::mutex> playerLock(clientMutex);
        std::unique_lock<std::mutex> alienBossLock(alienBossMutex);
        for (auto &entry : endpointToEntityMap) {
            Entity player = entry.second;
            bool isAlive = gCoordinator.GetComponent<SpriteComponent>(player).isAlive;
            if (isAlive) {

                for (Entity alienBoss : gameStruct->enemiesAlienBossVector) {
                    CheckCollision(alienBoss, player, gameStruct, true);
                }
            }
        }
    } 
}

void CheckCollision(Entity entity1, Entity entity2, struct GameStruct* gameStruct, bool isBoss)
{
    PositionComponent& position1 = gCoordinator.GetComponent<PositionComponent>(entity1);
    SpriteComponent& sprite1 = gCoordinator.GetComponent<SpriteComponent>(entity1);
    PositionComponent& position2 = gCoordinator.GetComponent<PositionComponent>(entity2);
    SpriteComponent& sprite2 = gCoordinator.GetComponent<SpriteComponent>(entity2);

    // Calculate bounds for both entities.
    sf::FloatRect bounds1 = CalculateBounds(position1, sprite1);
    sf::FloatRect bounds2 = CalculateBounds(position2, sprite2);

    // Perform collision detection logic.
    if (CheckBounds(bounds1, bounds2)) {
        if (isBoss)
            HandleCollisionBoss(entity1, entity2, gameStruct);
        else
            HandleCollision(entity1, entity2, gameStruct);
    }
}

sf::FloatRect CalculateBounds(const PositionComponent& position, const SpriteComponent& sprite) {
	sf::FloatRect bounds;
	bounds.left = position.position.x;
	bounds.top = position.position.y;
	bounds.width = sprite.size.x;
	bounds.height = sprite.size.y;
	return bounds;
}

bool CheckBounds(const sf::FloatRect& bounds1, const sf::FloatRect& bounds2) {
	return bounds1.intersects(bounds2);
}

void HandleCollision(Entity entity1, Entity entity2, struct GameStruct* gameStruct) {
	std::cout << "There is a collision!" << std::endl;
	gameStruct->damageSystem->TakeDamage(10, entity1);
	gameStruct->damageSystem->TakeDamage(1, entity2);
}

void HandleCollisionBoss(Entity entity1, Entity entity2, struct GameStruct* gameStruct) {
	std::cout << "There is a collision!" << std::endl;
	gameStruct->damageSystem->TakeDamage(1, entity1);
	gameStruct->damageSystem->TakeDamage(1, entity2);
}