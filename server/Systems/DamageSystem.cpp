#include "DamageSystem.hpp"

#include "../Components/LifeComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Core/Coordinator.hpp"
#include "../Maths/Vec2.hpp"
#include "../Mutex.hpp"

extern Coordinator gCoordinator;

void DamageSystem::TakeDamage(int damage, Entity entity)
{
	auto& life = gCoordinator.GetComponent<LifeComponent>(entity);

	life.remainingLife -= damage;

	if (life.remainingLife <= 0) {
		auto& sprite = gCoordinator.GetComponent<SpriteComponent>(entity);
		sprite.isAlive = false;
	}
}

void DamageSystem::HealDamage(int heal, Entity entity)
{
	auto& life = gCoordinator.GetComponent<LifeComponent>(entity);

	life.remainingLife += heal;

	auto& sprite = gCoordinator.GetComponent<SpriteComponent>(entity);
	sprite.isAlive = true;
}
