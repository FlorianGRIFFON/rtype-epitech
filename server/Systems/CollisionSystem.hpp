#pragma once

#include <vector>

#include "../Core/System.hpp"
#include "../Maths/Vec2.hpp"


class CollisionSystem : public System
{
public:
	CollisionSystem() {};

	void Update(struct GameStruct* gameStruct);
	
};
