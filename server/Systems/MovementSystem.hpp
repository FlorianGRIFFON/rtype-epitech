#pragma once

#include "../Core/System.hpp"
#include "../Maths/Vec2.hpp"

extern struct GameStruct* gameStruct;

class MovementSystem : public System
{
public:
	MovementSystem() {};

	void Move(Vec2 pos, Entity entity);
	void Set(Vec2 pos, Entity entity);
	void UpdateEnemies(struct GameStruct* gameStruct);
};