#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../Maths/Vec2.hpp"

struct SpriteComponent
{
    Vec2 size;
    int texturePath;
    int id;
    bool isAlive;

    SpriteComponent(Vec2 sizee, int path, int index, bool alive) {
        size = sizee;
        texturePath = path;
        id = index;
        isAlive = alive;
    }

    SpriteComponent() {
        size = Vec2(0.0f, 0.0f);
        texturePath = 0;
        id = -1;
        isAlive = false;
    }
};

