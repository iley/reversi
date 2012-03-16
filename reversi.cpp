#include <stdio.h>
#include <SDL/SDL.h>
#include <sexpress/iexcept.hpp>
#include "gamefield.hpp"
#include "reversi.hpp"
#include "player.hpp"

SDL_Surface *screen = NULL;
GameField field(8, 8);
Player *players[2];
int currentPlayer;

class Window
{
public:
    Window();

    void HandleEvents(SDL_Event &event);
    void Caption(const char *text);
    void Update();
};

Window window;

void UpdateCaption()
{
    static char buffer[64];
    sprintf(buffer, "Reversi (player %d move)", currentPlayer + 1);
    window.Caption(buffer);
}

Window::Window()
{
    screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, ScreenBpp, SDL_SWSURFACE);

    if (screen == NULL)
        throw IntelibX("Could not set video mode");
}

void Window::HandleEvents(SDL_Event &event)
{
    switch (event.type) {

    case SDL_VIDEORESIZE:
        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, ScreenBpp, SDL_SWSURFACE | SDL_DOUBLEBUF);
        break;

    case SDL_VIDEOEXPOSE:
        Update();
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
            int row = event.button.x / (ScreenHeight / field.Rows());
            int col = event.button.y / (ScreenWidth / field.Cols());

            HumanPlayer::HandleInput(row, col);
        }
        break;
    }
}

void Window::Caption(const char *text)
{
    SDL_WM_SetCaption(text, NULL);
}

void Window::Update()
{
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0xB0, 0x00));
    field.Draw(screen);
    SDL_Flip(screen);
}

bool moveCallback(int row, int col)
{
    if (field.matrix(row, col) == CHIP_NONE) {
        field.matrix(row,col) = players[currentPlayer]->Color();
        window.Update();
        currentPlayer = 1 - currentPlayer;
        UpdateCaption();
        players[currentPlayer]->Move(field, moveCallback);
        return true;
    } else {
        return false;
    }
}

bool keyPressed(const SDL_Event &event, int key)
{
    return event.type == SDL_KEYDOWN && event.key.keysym.sym == key;
}

void init()
{
    players[0] = new HumanPlayer(CHIP_WHITE);
    players[1] = new HumanPlayer(CHIP_BLACK);

    if (SDL_Init( SDL_INIT_EVERYTHING ) == -1)
        throw IntelibX("Couldn't initialize video");
}

void deinit()
{
    SDL_Quit();
    delete players[1];
    delete players[0];
}

int main()
{
    SDL_Event event;
    bool quit = false;

    init();

    currentPlayer = 0;
    UpdateCaption();
    players[currentPlayer]->Move(field, moveCallback);

    while (!quit && SDL_WaitEvent(&event)) {
        window.HandleEvents(event);
        window.Update(); //dirty hack

        if (keyPressed(event, SDLK_ESCAPE) || event.type == SDL_QUIT)
            quit = true;
    }

    deinit();

    return 0;
}
