#pragma once

#include "ECS.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
    SDL_Texture* texture{ nullptr };
    Vector2D position{ 0.0f, 0.0f };
    SDL_Rect srcRect{ 0, 0, 0, 0 }, destRect{ 0, 0, 0, 0 };

    TileComponent() = default;

    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xpos, int ypos, const char* path)
        : position{ xpos, ypos }, srcRect{ srcX, srcY }
    {
        texture = TextureManager::LoadTexture(path);
        srcRect.w = srcRect.h = 32;
        destRect.w = destRect.h = 64;
    }

    void update() override
    {
        // TODO: operator- that handles SDL_Rect?
        destRect.x = static_cast<int>(position.x) - Game::camera.x;
        destRect.y = static_cast<int>(position.y) - Game::camera.y;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};
