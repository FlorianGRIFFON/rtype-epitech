#pragma once

#include <vector>
#include <map>

#include "../Core/System.hpp"
#include "../Maths/Vec2.hpp"

extern struct GameStruct* gameStruct;


class ProjectileSystem : public System
{
public:
	ProjectileSystem() {};

	void ProjectileRoutine(struct GameStruct* gameStruct);
};
