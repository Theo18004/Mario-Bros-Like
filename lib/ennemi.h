#ifndef ENNEMI_H
#define ENNEMI_H

#include <SDL.h>
#include "player.h" // Pour réutiliser PlayerState

typedef struct {
    SDL_Rect rect;
    float velY;
    float speed;
    int direction;    // 1 pour droite, -1 pour gauche
    int onGround;
    PlayerState state;
    int vivant;       // 1 = actif, 0 = mort/disparu
} Ennemi;

void init_ennemi(Ennemi* e, int x, int y);
void update_ennemi(Ennemi* e, int* map);
void render_ennemi(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun);

#endif