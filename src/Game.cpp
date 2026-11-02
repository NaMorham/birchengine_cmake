#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

#ifndef BIRCH_RESOURCE_DIR
#error "BIRCH_RESOURCE_DIR must be defined by the build (see CMakeLists.txt)"
#endif

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,1600,1280 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;
bool Game::showColliders{ false };
bool Game::showBoundaries{ false };

auto& player(manager.addEntity());
auto& label(manager.addEntity());

Game::Game() : window{ nullptr }
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;

    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Use dx 12 on windows
#if defined(_WIN32) || defined(_WIN64)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12");
#endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        isRunning = true;
    }

    if (TTF_Init() == -1)
    {
        std::cout << "Error : SDL_TTF" << std::endl;
    }

    assets->AddTexture("terrain", resourcePath("assets/terrain_ss.png").c_str());
    assets->AddTexture("player", resourcePath("assets/player_anims.png").c_str());
    assets->AddTexture("projectile", resourcePath("assets/proj.png").c_str());

    assets->AddFont("LiberationSans", resourcePath("fonts/LiberationSans-Regular.ttf"), 16);

    map = new Map("terrain", 3, 32);
    //ecs implementation

    map->LoadMap(resourcePath("assets/map.map"), 25, 20);

    (void)player.addComponent<TransformComponent>(800.0f, 640.0f, 32 , 32, 2);
    (void)player.addComponent<SpriteComponent>("player", true);
    (void)player.addComponent<KeyboardController>();
    (void)player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    SDL_Color white = { 255, 255, 255, 255 };

    (void)label.addComponent<UILabel>(10, 10, "Test String", "LiberationSans", white);

    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,0),200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT :
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    std::stringstream ss;
    ss << "Player position: " << playerPos;
    label.getComponent<UILabel>().SetLabelText(ss.str(), "LiberationSans");

    manager.refresh();
    manager.update();

    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(cCol, playerCol))
        {
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    for (auto& p : projectiles)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Hit player!" << std::endl;
            p->destroy();
        }
    }

    camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 400.0f);
    camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320.0f);

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > camera.w)
        camera.x = camera.w;
    if (camera.y > camera.h)
        camera.y = camera.h;
}

void Game::render()
{
#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
    size_t idx{ 1 };
#endif
    // Draw colour is shared renderer state and is changed by debug overlays
    // (drawBounds), so set the clear colour explicitly every frame.
    // Results discarded: a failed clear only affects this frame, and logging
    // it here would repeat every frame.
    (void)SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    (void)SDL_RenderClear(renderer);
    for (auto& t : tiles)
    {
#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
        std::cerr << "Draw tile " << idx++ << "/" << tiles.size() << std::endl;
#endif
        t->draw();
        if (showBoundaries) t->drawBounds();
    }

    if (showColliders)
    {
#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
        idx=1;
#endif
        for (auto& c : colliders)
        {
#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
            std::cerr << "Draw collider " << idx++ << "/" << colliders.size() << std::endl;
#endif
            c->draw();
        }
    }

#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
    idx = 1;
#endif
    for (auto& p : players)
    {
#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
        std::cerr << "Draw players " << idx++ << "/" << players.size() << std::endl;
#endif
        p->draw();
        if (showBoundaries) p->drawBounds();
    }

#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
    idx = 1;
#endif
    for (auto& p : projectiles)
    {
#if (defined(_DEBUG) || defined(DEBUG)) && defined(_CHECK_RENDER)
        std::cerr << "Draw projectiles " << idx++ << "/" << projectiles.size() << std::endl;
#endif
        p->draw();
        if (showBoundaries) p->drawBounds();
    }

    label.draw();

    SDL_RenderPresent(renderer);
}

std::string Game::resourcePath(const std::string& relativePath)
{
    // Resolved once. SDL_GetBasePath() returns the executable's directory
    // (or Contents/Resources/ inside a macOS bundle), with a trailing separator.
    static const std::string resourceRoot = []()
    {
        std::string root;
        char* basePath = SDL_GetBasePath();
        if (basePath)
        {
            root = basePath;
            SDL_free(basePath);
        }
        else
        {
            // Fall back to the current working directory (the old behaviour).
            std::cerr << "SDL_GetBasePath failed: " << SDL_GetError() << std::endl;
        }
        return root + BIRCH_RESOURCE_DIR;
    }();

    return resourceRoot + relativePath;
}

void Game::clean()
{
    // Order matters: textures and fonts before the renderer and TTF_Quit,
    // and the renderer before its window.
    // Entities still hold non-owning texture pointers after this; they MUST
    // NOT be drawn again.
    delete assets;
    assets = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    TTF_Quit();
    SDL_Quit();
}
