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
    bool InputEnabled;

    Window();

    void HandleEvents(SDL_Event &event);
    void Caption(const char *text);
    void Update();
};

Window window;

void UpdateCaption()
{
    static char buffer[64];
    const char *color = (currentPlayer == 0 ? "white's" : "black's");
    sprintf(buffer, "Reversi (%s turn)", color);
    window.Caption(buffer);
}

Window::Window() : InputEnabled(true)
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
        if (InputEnabled && event.button.button == SDL_BUTTON_LEFT) {
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

void finishGame()
{
    window.InputEnabled = false;

    int whiteScore = field.Score(CHIP_WHITE),
        blackScore = field.Score(CHIP_BLACK);

    const char *message;
    if (whiteScore > blackScore)
        message = "The winner is white";
    else if (blackScore > whiteScore)
        message = "The winner is black";
    else
        message = "Draw";

    window.Caption(message);
}

bool moveCallback(int row, int col)
{
    int color = players[currentPlayer]->Color();
    if (field.Move(color, row, col)) {
        window.Update();

        int nextPlayer = 1 - currentPlayer;

        if (field.HasMoves(players[nextPlayer]->Color())) {
            currentPlayer = nextPlayer;
            UpdateCaption();
            players[currentPlayer]->Move(field, moveCallback);
        } else if (field.HasMoves(players[currentPlayer]->Color())) {
            UpdateCaption();
            players[currentPlayer]->Move(field, moveCallback);
        } else {
            finishGame();
        }

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
    if (SDL_Init( SDL_INIT_EVERYTHING ) == -1)
        throw IntelibX("Couldn't initialize video");

    players[0] = new HumanPlayer(CHIP_WHITE);
    players[1] = new HumanPlayer(CHIP_BLACK);
    currentPlayer = 0;

    UpdateCaption();

    players[currentPlayer]->Move(field, moveCallback);
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

    while (!quit && SDL_WaitEvent(&event)) {
        window.HandleEvents(event);
        window.Update(); //dirty hack

        if (keyPressed(event, SDLK_ESCAPE) || event.type == SDL_QUIT)
            quit = true;
    }

    deinit();

    return 0;
}
