#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "SDL_ttf.h"

class AssetManager
{
public:
    AssetManager(Manager* man);
    // Owns every texture and font it loads, and frees them on destruction.
    // MUST be destroyed before the renderer and before TTF_Quit().
    ~AssetManager();

    // Owns raw SDL resources, so copy and move are deleted (rule of five).
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

    //gameobjects

    void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);

    //texture management
    void AddTexture(std::string id, const char* path);
    SDL_Texture* GetTexture(std::string id);

    void AddFont(std::string id, std::string path, int fontSize);
    TTF_Font* GetFont(std::string id);

private:
    Manager* manager{ nullptr };
    std::map<std::string, SDL_Texture*> textures{};
    std::map<std::string, TTF_Font*> fonts{};
};
