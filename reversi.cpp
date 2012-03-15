#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <sexpress/iexcept.hpp>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

SDL_Surface *screen = NULL;

class Window
{
public:
    Window();

    void handleEvents(SDL_Event &event);
    void caption(const char *text);
};

Window::Window()
{
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);

    if (screen == NULL)
        throw IntelibX("Could not set video mode");

    caption("Window Event Test");
}

void Window::handleEvents(SDL_Event &event)
{
    if (event.type == SDL_VIDEORESIZE) {
        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);

        if (screen == NULL)
            throw IntelibX("Could not resize the window");
    } else if (event.type == SDL_ACTIVEEVENT) {
        if (event.active.state & SDL_APPACTIVE)
        {
            if( event.active.gain == 0 )
                caption("Window Event Test: Iconified");
            else
                caption("Window Event Test");

        } else if (event.active.state & SDL_APPINPUTFOCUS) {
            if (event.active.gain == 0)
                caption("Window Event Test: Keyboard focus lost");
            else
                caption("Window Event Test");

        } else if (event.active.state & SDL_APPMOUSEFOCUS) {
            if (event.active.gain == 0)
                caption("Window Event Test: Mouse Focus Lost");
            else
                caption("Window Event Test");
        }
    } else if (event.type == SDL_VIDEOEXPOSE) {
        if( SDL_Flip( screen ) == -1 )
            throw IntelibX("Fuck");
    }
}

void Window::caption(const char *text)
{
    SDL_WM_SetCaption(text, NULL);
}

bool keyPressed(const SDL_Event &event, int key)
{
    return event.type == SDL_KEYDOWN && event.key.keysym.sym == key;
}

bool init()
{
    if (SDL_Init( SDL_INIT_EVERYTHING ) == -1)
        return false;

    return true;
}

void deinit()
{
    SDL_Quit();
}

int main()
{
    SDL_Event event;
    bool quit = false;

    if( init() == false )
        return 1;

    Window myWindow;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            myWindow.handleEvents(event);

            if (keyPressed(event, SDLK_ESCAPE) || event.type == SDL_QUIT)
                quit = true;
        }

        SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

        if (SDL_Flip(screen) == -1)
            return 1;
    }

    deinit();

    return 0;
}
