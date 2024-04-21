#include <iostream>
#ifdef _MSC_FULL_VER
#define SDL_MAIN_HANDLED  // Linker error about main without this
#endif
#include <SDL.h>

void DumpArgs(const int argc, const char* const * argv);

int main(int argc, char *argv[])
{
	SDL_Window* pWindow = nullptr;
	SDL_Renderer* pRenderer = nullptr;

	DumpArgs(argc, argv);

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_EVERYTHING);

	pWindow = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN);
	if (!pWindow)
	{
		return EXIT_FAILURE;
	}

	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	if (!pRenderer)
	{
		SDL_DestroyWindow(pWindow);
		pWindow = nullptr;
		return EXIT_FAILURE;
	}

	SDL_SetRenderDrawColor(pRenderer, 0, 0x0f, 0x7f, 0xff);

	SDL_RenderClear(pRenderer);

	SDL_RenderPresent(pRenderer);

	SDL_Delay(5000);

	return 0;
}

void DumpArgs(const int argc, const char* const* argv)
{
	std::cerr << argv[0] << " called with " << argc-1
	    << (argc == 2 ? " argument." : " arguments.") << std::endl;

	for (int idx = 1; idx < argc; idx++)
	{
		std::cerr << "\t" << idx << ": '" << *(argv + idx) << "'" << std::endl;
	}
	std::cerr << std::endl;
}
