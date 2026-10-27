#include "Game.h"

Game *game = nullptr;

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        // TODO: Handle cli params to show collision tiles?
        for (int argIdx = 1; argIdx < argc; ++argIdx) {
            std::cerr << "Arg " << argIdx << " '" << argv[argIdx] << "'" << std::endl;
        }
    }
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    game->init("GameWindow", 800, 640, false);

    while (game->running())
    {

        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }

    }

    game->clean();
    return 0;
}
