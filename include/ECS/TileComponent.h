#pragma once

#include "ECS.h"
#include "SDL.h"
#include "../AssetManager.h"

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

    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
        : position{ xpos, ypos }, srcRect{ srcX, srcY }, destRect{ xpos, ypos }
    {
        texture = Game::assets->GetTexture(id);
        srcRect.w = srcRect.h = tsize;
        destRect.w = destRect.h = tsize * tscale;
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
