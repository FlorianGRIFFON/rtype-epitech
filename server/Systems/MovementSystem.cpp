#include "MovementSystem.hpp"

#include "../Components/PositionComponent.hpp"
#include "../Core/Coordinator.hpp"
#include "../Maths/Vec2.hpp"

#include "../Mutex.hpp"
#include "../config.hpp"

extern Coordinator gCoordinator;
int isPause = 0;
int isShooting = 0;
int posYstraight;

void MovementSystem::Move(Vec2 pos, Entity entity)
{
	bool alive = gCoordinator.GetComponent<SpriteComponent>(entity).isAlive;

    if (alive == false)
        return;
	
	auto& position = gCoordinator.GetComponent<PositionComponent>(entity);
    auto& spriteCode = gCoordinator.GetComponent<SpriteComponent>(entity).texturePath;

    if (spriteCode == 1) {
        if ((position.position.x + pos.x) < 0 || (position.position.x + pos.x) > SCREEN_WIDTH - 66) {
            
        } else
			position.position.x += pos.x;
		if ((position.position.y + pos.y) < 0 || (position.position.y + pos.y) > SCREEN_HEIGHT - 34) {

		} else
			position.position.y += pos.y;
    } else {
		position.position.x += pos.x;
		position.position.y += pos.y;
	}
}

void MovementSystem::Set(Vec2 pos, Entity entity)
{
	auto& position = gCoordinator.GetComponent<PositionComponent>(entity);

	position.position.x = pos.x;
	position.position.y = pos.y;
}

void MovementSystem::UpdateEnemies(struct GameStruct* gameStruct)
{
	// Mosquito
	{
		std::unique_lock<std::mutex> mosquitoLock(mosquitoMutex);

		if (!gameStruct->enemiesMosquitoVector.empty()) {
			auto it = gameStruct->enemiesMosquitoVector.begin();
			while (it != gameStruct->enemiesMosquitoVector.end()) {
				const auto& enemy = *it;
				PositionComponent& position = gCoordinator.GetComponent<PositionComponent>(enemy);
				SpriteComponent& sprite = gCoordinator.GetComponent<SpriteComponent>(enemy);
				InitialPositionComponent& initialPosition = gCoordinator.GetComponent<InitialPositionComponent>(enemy);

				sf::CircleShape circleShape(sprite.size.x / 2.0f);
				circleShape.setPosition(position.position.x, position.position.y);
				gameStruct->movementSystem->Move(Vec2(-5.0f, 0.0f), enemy);

				if (initialPosition.move == 0) {
					gameStruct->movementSystem->Move(Vec2(-1.0f, -4.0f), enemy);
				} else if (initialPosition.move == 1) {
					gameStruct->movementSystem->Move(Vec2(-1.0f, 4.0f), enemy);
				}

				if ((position.position.y <= initialPosition.position.y - 50) && initialPosition.move == 0)
					initialPosition.move = 1;
				else if ((position.position.y >= initialPosition.position.y + 50) && initialPosition.move == 1)
					initialPosition.move = 0;

				if (position.position.x < -60) {
					std::cout << "Mosquito Leave" << std::endl;
					gameStruct->damageSystem->TakeDamage(10, enemy);
				}
				++it;

			}
		}
	}

	// Plane movement
	{
		std::unique_lock<std::mutex> planeLock(planeMutex);
		if (!gameStruct->enemiesPlaneVector.empty()) {
			auto it = gameStruct->enemiesPlaneVector.begin();
			while (it != gameStruct->enemiesPlaneVector.end()) {
				const auto& enemy = *it;
				PositionComponent& position = gCoordinator.GetComponent<PositionComponent>(enemy);
				InitialPositionComponent& initialPosition = gCoordinator.GetComponent<InitialPositionComponent>(enemy);

				gameStruct->movementSystem->Move(Vec2(-3.0f, 0.0f), enemy);

				if ((position.position.x <= SCREEN_WIDTH * 0.75f) && (initialPosition.move == 0)) {
					std::unique_lock<std::mutex> planeProjectileLock(planeProjectileMutex);
					initialPosition.move = 1;
					createPlaneProjectile(gameStruct, gCoordinator, position.position);
				}

				if ((position.position.x <= SCREEN_WIDTH * 0.25f) && (initialPosition.move == 1)) {
					std::unique_lock<std::mutex> planeProjectileLock(planeProjectileMutex);
					createPlaneProjectile(gameStruct, gCoordinator, position.position);
					initialPosition.move = 2;
				}

				if (position.position.x < -110) {
					std::cout << "Plane Leave" << std::endl;
					gameStruct->damageSystem->TakeDamage(10, enemy);
				}
				++it;
			}
		}
	}

	// AlienBoss movement
	{
		std::unique_lock<std::mutex> alienBossLock(alienBossMutex);
		if (!gameStruct->enemiesAlienBossVector.empty()) {
			auto it = gameStruct->enemiesAlienBossVector.begin();
			while (it != gameStruct->enemiesAlienBossVector.end()) {
				const auto& enemy = *it;
				PositionComponent& position = gCoordinator.GetComponent<PositionComponent>(enemy);

				if (position.position.x > SCREEN_WIDTH - 100) {
					gameStruct->movementSystem->Move(Vec2(-2.0f, 0.0f), enemy);
				} else {
					if (isPause > 0) {
						isPause--;
					} else {
						if (isShooting > 0) {
							isShooting--;

							int posY = rand() % SCREEN_HEIGHT;
							std::unique_lock<std::mutex> planeProjectileLock(planeProjectileMutex);
							createPlaneProjectile(gameStruct, gCoordinator, Vec2(position.position.x, position.position.y + posY));
						} else {
							isShooting = 20;
							isPause = 60;
							posYstraight = rand() % SCREEN_HEIGHT;
						}
					}
					if (isPause > 0 && isPause % 2 == 0) {
						int doIShoot = rand() % 5;
						if (doIShoot == 1) {
							std::unique_lock<std::mutex> straightProjectileLock(straightProjectileMutex);
							createStraightProjectile(gameStruct, gCoordinator, Vec2(position.position.x, position.position.y + posYstraight));
						}
					}
				}

				++it;
			}
		}
	}
}
