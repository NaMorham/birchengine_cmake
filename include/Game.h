#pragma once

#ifdef _MSC_FULL_VER
#define SDL_MAIN_HANDLED
#endif
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <filesystem>

namespace Birch
{
    class Game
    {
    public:
        Game(std::filesystem::path &exePath);
        ~Game();

        bool init(const char* title, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        bool running() { return m_isRunning; }
        void render();
        void clean();

    private:
        std::filesystem::path& m_exePath;
        std::string m_title{ "" };
        bool m_isRunning{ false };
        int m_width{ 1000 };
        int m_height{ 1000 };
        int m_renderCount{ 0 };
        SDL_Window* m_pWindow{ nullptr };
        SDL_Renderer* m_pRenderer{ nullptr };
        Uint64 m_desiredFps{ 64 };
        Uint64 m_msPerFrame{ 16 };
        Uint64 m_lastFrameTime{ 0 };
        Uint64 m_deltaTicks{ 0 };
        float m_deltaTime{ 0 };

        TTF_Font* m_pCourierFont{ nullptr };
        SDL_Surface* m_pFontSurface{ nullptr };
        SDL_Rect m_textRect{ 0, 0, 0, 0 };
        SDL_Texture* m_pTextTexture{ nullptr };
    };

}; // end namespace Birch
