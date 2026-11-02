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

    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
        : srcRect{ srcX, srcY }, destRect{ xpos, ypos }, 
          position{ static_cast<float>(xpos), static_cast<float>(ypos) }
    {
        texture = Game::assets->GetTexture(id);
        srcRect.w = srcRect.h = tsize;
        destRect.w = destRect.h = tsize * tscale;
    }

    void update() override
    {
        destRect.x = static_cast<int>(position.x - Game::camera.x);
        destRect.y = static_cast<int>(position.y - Game::camera.y);
    }

    void draw() const override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }

    void drawBounds() const override
    {
        // Results discarded: a failed debug-overlay draw is non-fatal, and
        // logging it here would repeat every frame for every entity.
        (void)SDL_SetRenderDrawColor(Game::renderer, 0x00, 0x00, 0x00, 0x7f);
        (void)SDL_RenderDrawRect(Game::renderer, &destRect);
    }
};
