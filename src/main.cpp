#include <filesystem>
#include "Game.h"

Birch::Game *game = nullptr;

int main(int argc, char *argv[])
{
    SDL_SetMainReady();

    // calculate the absolute path to the executable so we can locate the fonts folder
    std::filesystem::path arg0(argv[0]);
    if (arg0.is_relative()) {
        arg0 = std::filesystem::canonical(arg0);
    }
#if defined(_DEBUG) || defined(DEBUG)
    std::cerr << "arg0: " << arg0 << std::endl;
#endif

    game = new Birch::Game(arg0);
    if (!game->init("GameWindow", 800, 600, false))
    {
        std::cerr << "Failed to initialise: " << SDL_GetError() << std::endl;
    }
    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();
    return 0;
}
