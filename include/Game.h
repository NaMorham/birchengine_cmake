#pragma once

#ifdef _MSC_FULL_VER
#define SDL_MAIN_HANDLED
#endif
#include "SDL.h"
#include <iostream>
#include <string>

namespace Birch
{
    class Game
    {
    public:
        Game();
        ~Game();

        void init(const char* title, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        bool running() { return isRunning; }
        void render();
        void clean();

    private:
        std::string m_title;
        bool isRunning = false;
        int cnt = 0;
        SDL_Window* window;
        SDL_Renderer* renderer;
    };

}; // end namespace Birch
