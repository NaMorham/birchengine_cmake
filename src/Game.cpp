#include <format>
#include "../include/Game.h"

namespace Birch
{

    Game::Game(std::filesystem::path& exePath) : 
        m_pRenderer(nullptr), m_pWindow(nullptr), m_title(""), m_exePath{ exePath }
    {
        std::cerr << "exe path: " << m_exePath << std::endl;
    }

    Game::~Game()
    {
    }

    bool Game::init(const char* title, int width, int height, bool fullscreen)
    {
        int flags = 0;
        m_isRunning = false;
        m_width = width;
        m_height = height;

        if (fullscreen)
        {
            flags = SDL_WINDOW_FULLSCREEN;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            std::cerr << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
            return false;
        }
        if (TTF_Init() != 0)
        {
            std::cerr << "Failed to initialise SDL fonts: " << SDL_GetError() << std::endl;
            return false;
        }

        m_pWindow = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (m_pWindow == nullptr)
        {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
            return false;
        }
        m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
        if (m_pRenderer == nullptr)
        {
            std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
            return false;
        }
        std::filesystem::path fontPath(m_exePath.parent_path() / "Resources" / "fonts" / "courbd.ttf");
        m_pCourierFont = TTF_OpenFont(fontPath.make_preferred().string().c_str(), 12);
        if (m_pCourierFont == nullptr)
        {
            std::cerr << "Failed to open font: " << SDL_GetError() << std::endl;
            return false;
        }

        m_msPerFrame = (60.0f / static_cast<float>(m_desiredFps)) * 1000.0f;

        m_isRunning = true;
        return true;
    }

    void Game::handleEvents()
    {
        SDL_Event event;
        SDL_Scancode scanCode;

        SDL_PollEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            m_isRunning = false;
            break;
        case SDL_KEYDOWN:
            break;
        case SDL_KEYUP:
            scanCode = event.key.keysym.scancode;
            switch (scanCode)
            {
            case SDL_SCANCODE_ESCAPE:
            case SDL_SCANCODE_R:
                m_isRunning = false;
                break;
            }
            break;
        default:
            break;
        }
    }

    void Game::update()
    {
        Uint64 tc = SDL_GetTicks64();
        m_renderCount++;

        this->m_deltaTicks = tc - this->m_lastFrameTime;
        this->m_deltaTime = static_cast<float>(this->m_deltaTicks) / 1000.0;

        // TODO: Add any actual update of game objects

        this->m_lastFrameTime = tc;
        if (this->m_deltaTicks < this->m_msPerFrame)
        {
            SDL_Delay(this->m_msPerFrame - this->m_deltaTicks);
        }
    }

    void Game::render()
    {
        if (!m_pRenderer)
        {
            std::cerr << "Missing or invalid renderer: " << SDL_GetError() << std::endl;
            m_isRunning = false;
            return;
        }
        if (SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 16, 255))
        {
            std::cerr << "Failed to set Game render draw colour with error: " << SDL_GetError() << std::endl;
            return;
        }
        if (SDL_RenderClear(m_pRenderer))
        {
            std::cerr << "Failed to clear renderer with error: " << SDL_GetError() << std::endl;
            return;
        }

        // Render text
        if (true) { // TODO: Update this check when we have a test for displaying
            SDL_RenderSetScale(m_pRenderer, 1, 1);

            float fps{ 60.0f / m_deltaTime };

            std::string message{ std::format("Desired fps: {}, delta ticks: {}, delta time: {}, fps: {}",
                m_desiredFps, m_deltaTicks, m_deltaTime, fps) };

            m_pFontSurface = TTF_RenderText_Blended_Wrapped(m_pCourierFont, message.c_str(),
                SDL_Color{ 255, 255, 255, 255 }, m_width - 20);
            if (m_pTextTexture) {
                SDL_DestroyTexture(m_pTextTexture);
            }
            m_pTextTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pFontSurface);
            if (!m_pTextTexture) {
                std::cerr << "Failed to create texture" << std::endl;
                m_isRunning = false;
                return;
            }
            m_textRect = { 10, 10, m_pFontSurface->w, m_pFontSurface->h };
            SDL_FreeSurface(m_pFontSurface);
            m_pFontSurface = nullptr;
            SDL_RenderCopy(m_pRenderer, m_pTextTexture, NULL, &m_textRect);
        }

        SDL_RenderPresent(m_pRenderer);
    }

    void Game::clean()
    {
        if (m_pCourierFont) { TTF_CloseFont(m_pCourierFont); m_pCourierFont = nullptr; }
        if (m_pRenderer) { SDL_DestroyRenderer(m_pRenderer); m_pRenderer = nullptr; }
        if (m_pWindow) { SDL_DestroyWindow(m_pWindow); m_pWindow = nullptr; }
        TTF_Quit();
        SDL_Quit();
    }

}; // end namespace Birch
