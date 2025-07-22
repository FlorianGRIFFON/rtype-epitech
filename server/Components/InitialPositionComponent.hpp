#pragma once

#include <iostream>

#include "../Maths/Vec2.hpp"

struct InitialPositionComponent {

    Vec2 position;
    int move;

    InitialPositionComponent() {
        position.x = 0.0;
        position.y = 0.0;
        move = 0;
    }

    InitialPositionComponent(float x, float y) {
        position.x = x;
        position.y = y;
        move = 0;
    }

    InitialPositionComponent(Vec2 pos) {
        position.x = pos.x;
        position.y = pos.y;
        move = 0;
    }
};
