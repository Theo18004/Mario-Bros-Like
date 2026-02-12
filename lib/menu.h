#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum {
    STATE_MENU,
    STATE_PLAY,
    STATE_EXIT
} GameState;

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    const char* text;
    SDL_Color color;
} Button;

// Initialise un bouton
Button createButton(int x, int y, int w, int h, const char* text, SDL_Texture* tex);

// Dessine le menu complet
void renderMenu(SDL_Renderer* renderer, Button* buttons, int count, TTF_Font* font);

#endif