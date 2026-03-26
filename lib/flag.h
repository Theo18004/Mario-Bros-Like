#ifndef FLAG_H
#define FLAG_H

#include <SDL2/SDL.h>

// --- Structure Flag ---
typedef struct Flag{
    SDL_Rect matRect;   // Le mât
    SDL_Rect flagRect;  // Le tissu du drapeau
    int actif;          // 0 = en haut, 1 = descend, 2 = en bas
} Flag;

// --- Prototypes des fonctions ---
void init_flag(Flag* f, int x, int y);
void update_flag(Flag* f); // dt est le temps écoulé (delta time)
void render_flag(SDL_Renderer* renderer, Flag* f, SDL_Texture* texMat,
     SDL_Texture* texDrapeau, int camX, int camY);

#endif