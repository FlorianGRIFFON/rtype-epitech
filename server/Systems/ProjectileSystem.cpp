#include <vector>

#include "../Components/PositionComponent.hpp"
#include "../Core/Coordinator.hpp"
#include "../Maths/Vec2.hpp"

#include "ProjectileSystem.hpp"

#include "../../shared/Config.hpp"
#include "../config.hpp"
#include "../Mutex.hpp"

extern Coordinator gCoordinator;

void ProjectileSystem::ProjectileRoutine(struct GameStruct* gameStruct)
{
	{
		std::unique_lock<std::mutex> projectilesPlayerLock(projectilesPlayerMutex);

		// ProjectilesPlayer Routine
		for (int i = 0; i < gameStruct->projectilesPlayerVector.size(); i++) {
			gameStruct->movementSystem->Move(Vec2(5.0f, 0.0f), gameStruct->projectilesPlayerVector[i]);
			auto &pos = gCoordinator.GetComponent<PositionComponent>(gameStruct->projectilesPlayerVector[i]);
			if (pos.position.x > SCREEN_WIDTH + 30) {
				gameStruct->damageSystem->TakeDamage(10, gameStruct->projectilesPlayerVector[i]);
			}
		}
	}
	{
		std::unique_lock<std::mutex> straightProjectileLock(straightProjectileMutex);

		// StraightProjectile Routine
		for (int i = 0; i < gameStruct->projectilesStraightVector.size(); i++) {
			gameStruct->movementSystem->Move(Vec2(-5.0f, 0.0f), gameStruct->projectilesStraightVector[i]);
			auto &pos = gCoordinator.GetComponent<PositionComponent>(gameStruct->projectilesStraightVector[i]);
			if (pos.position.x < -30) {
				gameStruct->damageSystem->TakeDamage(10, gameStruct->projectilesStraightVector[i]);
			}
		}
	}
	{
		std::unique_lock<std::mutex> planeProjectileLock(planeProjectileMutex);
		
		// PlaneProjectile Routine
		auto it = gameStruct->projectilesPlaneVector.begin();

		while (it != gameStruct->projectilesPlaneVector.end()) {
			Entity& entity = *it;
			PositionComponent& position = gCoordinator.GetComponent<PositionComponent>(entity);
			ProjectileVectorComponent& projectileVector = gCoordinator.GetComponent<ProjectileVectorComponent>(entity);

			position.position.x += projectileVector.vector.x * 0.01f;
			position.position.y += projectileVector.vector.y * 0.01f;

			if (position.position.x < -50 || position.position.y < -50
			|| position.position.x > SCREEN_WIDTH + 50 || position.position.y > SCREEN_HEIGHT + 50)
				gameStruct->damageSystem->TakeDamage(10, entity);
			
			++it;
		}
	}

}
