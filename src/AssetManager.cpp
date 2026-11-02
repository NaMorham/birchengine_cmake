#include "AssetManager.h"
#include "ECS/Components.h"
#include <iostream>

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{
    // Map entries can be null (a failed load is still registered), so check
    // before freeing.
    for (auto& [id, texture] : textures)
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }
    for (auto& [id, font] : fonts)
    {
        if (font)
        {
            TTF_CloseFont(font);
        }
    }
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
    auto& projectile(manager->addEntity());
    (void)projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    (void)projectile.addComponent<SpriteComponent>(id, false);
    (void)projectile.addComponent<ProjectileComponent>(range, speed, vel);
    (void)projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
    textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(const std::string& id) const
{
    // find(), not operator[]: a missed lookup MUST NOT insert a null entry
    // (SC-16). The miss is reported here; the caller gets nullptr, which
    // SDL rejects as a draw source (nothing is drawn).
    const auto it = textures.find(id);
    if (it == textures.end())
    {
        std::cerr << "AssetManager: no texture registered as '" << id << "'" << std::endl;
        return nullptr;
    }
    return it->second;
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(const std::string& id) const
{
    // find(), not operator[]: see GetTexture().
    const auto it = fonts.find(id);
    if (it == fonts.end())
    {
        std::cerr << "AssetManager: no font registered as '" << id << "'" << std::endl;
        return nullptr;
    }
    return it->second;
}
