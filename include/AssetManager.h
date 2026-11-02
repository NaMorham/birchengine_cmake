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
    // Returns the texture registered as id, or nullptr (reported to stderr)
    // if there is none. Never inserts. The pointer is non-owning.
    [[nodiscard]] SDL_Texture* GetTexture(const std::string& id) const;

    void AddFont(std::string id, std::string path, int fontSize);
    // Returns the font registered as id, or nullptr (reported to stderr)
    // if there is none. Never inserts. The pointer is non-owning.
    [[nodiscard]] TTF_Font* GetFont(const std::string& id) const;

private:
    Manager* manager{ nullptr };
    std::map<std::string, SDL_Texture*> textures{};
    std::map<std::string, TTF_Font*> fonts{};
};
