#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include <string>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform{ nullptr };
    SDL_Texture *texture{ nullptr };
    SDL_Rect srcRect{ 0, 0, 0, 0 }, destRect{0, 0, 0, 0 };

    bool animated{ false };
    int frames{ 1 };
    int speed{ 100 };

public:
    int animIndex{ 0 };
    std::map<std::string, Animation> animations{};

    SDL_RendererFlip spriteFlip{ SDL_FLIP_NONE };

    SpriteComponent() = default;
    SpriteComponent(std::string id)
    {
        setTex(id);
    }

    SpriteComponent(std::string id, bool isAnimated)
    {
        animated = isAnimated;

        Animation idle = Animation(0, 3, 100);
        Animation walk = Animation(1, 8, 100);

        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);

        Play("Idle");

        setTex(id);
    }

    ~SpriteComponent()
    {
    }

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void init() override
    {

        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
        }

        srcRect.y = animIndex * transform->height;

        destRect.x = static_cast<int>(transform->position.x - Game::camera.x);
        destRect.y = static_cast<int>(transform->position.y - Game::camera.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() const override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(const std::string& animName)
    {
        // Use .at() so that play will not create a new entry if not found.
        // TODO: defensive check to ensure an animation WAS actually found.
        const Animation& anim = animations.at(animName);
        frames = anim.frames;
        animIndex = anim.index;
        speed = anim.speed;
    }

};
