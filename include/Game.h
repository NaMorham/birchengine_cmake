#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <string>
#include <vector>

class AssetManager;
class ColliderComponent;

class Game
{
public:
    Game();
    ~Game();

    void init(const char* title, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    bool running() const { return isRunning; }
    void render();
    void clean();

    // Full path to a resource, relative to the executable rather than the
    // current working directory. MUST NOT be called before SDL_Init.
    static std::string resourcePath(const std::string& relativePath);

    static SDL_Renderer* renderer;
    static SDL_Event event;
    static bool isRunning;
    static bool showColliders;
    static bool showBoundaries;

    static SDL_Rect camera;
    static AssetManager* assets;
    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayers,
        groupColliders,
        groupProjectiles
    };

private:
    int cnt{ 0 };
    SDL_Window* window{ nullptr };
};
