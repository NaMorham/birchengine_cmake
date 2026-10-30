#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:
    Vector2D position{ 0.0f, 0.0f };
    Vector2D velocity{ 0.0f, 0.0f };

    int height{ 32 };
    int width{ 32 };
    int scale{ 1 };
    int speed{ 3 };

    bool blocked = false;

    TransformComponent()
    {
        position.Zero();
        //velocity.Zero();
    }

    TransformComponent(int sc)
        : scale{ sc }
    {
        position.Zero();
        //velocity.Zero();
    }

    TransformComponent(float x, float y)
        : position{ x, y }
    {
        //velocity.Zero();
    }

    TransformComponent(float x, float y, int h, int w, int sc)
        : position{ x, y }, height{ h }, width{ w }, scale{ sc }
    {
    }

    void init() override
    {
        velocity.Zero();
    }
    void update() override
    {
        position.x += static_cast<int>(velocity.x * speed);
        position.y += static_cast<int>(velocity.y * speed);
    }

};
