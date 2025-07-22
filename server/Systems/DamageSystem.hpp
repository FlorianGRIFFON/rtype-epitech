#pragma once

#include "../Core/System.hpp"
#include "../Maths/Vec2.hpp"


class DamageSystem : public System
{
public:
	DamageSystem() {};

	void TakeDamage(int damage, Entity entity);
	void HealDamage(int heal, Entity entity);
};
