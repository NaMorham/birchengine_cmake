#pragma once

#include "ECS.h"
#include "../Vector2D.h"
#include "../Game.h"
#include "../TextureManager.h"

class TileComponent : public Component
{
public:
    SDL_Texture* texture{ nullptr };
    SDL_Rect srcRect{ 0, 0, 0, 0 }, destRect{ 0, 0, 0, 0 };
    Vector2D position{ 0.0f, 0.0f };

    TileComponent() = default;

    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, const char* path)
        : srcRect{ srcX, srcY }, destRect{ xpos, ypos }, 
          position{ static_cast<float>(xpos), static_cast<float>(ypos) }
    {
        texture = TextureManager::LoadTexture(path);
        srcRect.w = srcRect.h = tsize;
        destRect.w = destRect.h = tsize * tscale;
    }

    void update() override
    {
        destRect.x = static_cast<int>(position.x - Game::camera.x);
        destRect.y = static_cast<int>(position.y - Game::camera.y);
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};
