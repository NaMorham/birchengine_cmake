#pragma once

struct Animation
{
    int index{ 0 };
    int frames{ 1 };
    int speed{ 100 };

    Animation() = default;
    Animation(int i, int f, int s)
    {
        index = i;
        frames = f;
        speed = s;
    }
};
