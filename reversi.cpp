#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <sexpress/iexcept.hpp>
#include "gamefield.hpp"
#include "reversi.hpp"

SDL_Surface *screen = NULL;
GameField field(8, 8);

class Window
{
public:
    Window();

    void HandleEvents(SDL_Event &event);
    void Caption(const char *text);
    void Update();
};

Window::Window()
{
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if (screen == NULL)
        throw IntelibX("Could not set video mode");

    Caption("Window Event Test");
}

void Window::HandleEvents(SDL_Event &event)
{
    if (event.type == SDL_VIDEORESIZE) {
        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);

    } else if (event.type == SDL_VIDEOEXPOSE) {
        printf("expose\n");
        Update();
    }
}

void Window::Caption(const char *text)
{
    SDL_WM_SetCaption(text, NULL);
}

void Window::Update()
{
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    field.Draw(screen);
    SDL_Flip(screen);
}

bool keyPressed(const SDL_Event &event, int key)
{
    return event.type == SDL_KEYDOWN && event.key.keysym.sym == key;
}

void init()
{
    if (SDL_Init( SDL_INIT_EVERYTHING ) == -1)
        throw IntelibX("Couldn't initialize video");

    SDL_EventState(SDL_VIDEOEXPOSE, SDL_ENABLE);
}

void deinit()
{
    SDL_Quit();
}

int main()
{
    SDL_Event event;
    bool quit = false;

    init();

    Window window;

    while (!quit && SDL_WaitEvent(&event)) {
        window.HandleEvents(event);
        window.Update(); //dirty hack

        if (keyPressed(event, SDLK_ESCAPE) || event.type == SDL_QUIT)
            quit = true;
    }

    deinit();

    return 0;
}
