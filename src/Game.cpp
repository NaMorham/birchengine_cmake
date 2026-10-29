#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 1600, 1280 };

bool Game::isRunning = false;

auto& player(manager.addEntity());

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

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        isRunning = true;
    }

    map = new Map("Resources/assets/terrain_ss.png", 3, 32);
    //ecs implementation

    map->LoadMap("Resources/assets/map.map", 25, 20);

    player.addComponent<TransformComponent>(800.0f, 640.0f, 32, 32, 2);
    player.addComponent<SpriteComponent>("Resources/assets/player_anims.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

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
    SDL_RenderClear(renderer);
    for (auto& t : tiles)
    {
        t->draw();
    }

	for (auto& c : colliders)
	{
		c->draw();
	}

    for (auto& p : players)
    {
        p->draw();
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
