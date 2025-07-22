#pragma once

#include <iostream>
#include "../Maths/Vec2.hpp"

struct ProjectileVectorComponent {

    Vec2 vector;

    ProjectileVectorComponent() {
        vector.x = 0.0;
        vector.y = 0.0;
    }

    ProjectileVectorComponent(float x, float y) {
        vector.x = x;
        vector.y = y;
    }

    ProjectileVectorComponent(Vec2 pos) {
        vector.x = pos.x;
        vector.y = pos.y;
    }
};
