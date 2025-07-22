#pragma once

#include <iostream>

#include "../Maths/Vec2.hpp"

struct PositionComponent
{
	Vec2 position;

	PositionComponent() {
		position.x = 0.0;
		position.y = 0.0;
	}

	PositionComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}
	
	PositionComponent(Vec2 pos) {
		position.x = pos.x;
		position.y = pos.y;
	}
};
