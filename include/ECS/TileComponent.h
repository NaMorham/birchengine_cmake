#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
    TransformComponent *transform{ nullptr };
    SpriteComponent *sprite{ nullptr };

    SDL_Texture* texture{ nullptr };
    SDL_Rect srcRect{ 0, 0, 0, 0 }, destRect{ 0, 0, 0, 0 };

    TileComponent() = default;

    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xpos, int ypos, const char* path)
    {
        texture = TextureManager::LoadTexture(path);

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = 32;

        destRect.x = xpos;
        destRect.y = ypos;
        destRect.w = destRect.h = 64;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};
