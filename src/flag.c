/**
 * @file flag.c
 * @brief Gestion du joueur (états, physique et rendu).
 */

#include "flag.h"
#include <stdio.h>

// --- Variables d'animation ---
#define NUM_FRAMES_DESCENDING 8 // Hypothèse : 8 frames 
#define DESCEND_SPEED 150.0f     // Pixels par seconde

void init_flag(Flag* f, int x, int y) {
    // 1. On règle le Mât 
    f->matRect = (SDL_Rect){ x, y, 60, 250 };

    // 2. On règle le Dreapeau 
    // On le positionne en haut du mât (y) et collé à gauche du mât (x - 64)
    f->flagRect = (SDL_Rect){ x+36 , y+45 , 64, 64 }; 

    f->actif = 0;
}

void update_flag(Flag* f) {
    if (f->actif == 1) {
        // La limite basse : le bas du mât moins la hauteur du drapeau
        int yMax = f->matRect.y + f->matRect.h - f->flagRect.h;

        // Tant qu'on n'est pas en bas, on descend
        if (f->flagRect.y < yMax) {
            f->flagRect.y += 5; // Vitesse de chute 
        } else {
            f->flagRect.y = yMax; // On le cale parfaitement en bas
            f->actif = 2; // Fini 
        }
    }
}

void render_flag(SDL_Renderer* renderer, Flag* f, SDL_Texture* texMat, SDL_Texture* texDrapeau, int camX, int camY) {
    // 1. On affiche le mât
    SDL_Rect destMat = { f->matRect.x - camX, f->matRect.y - camY, f->matRect.w, f->matRect.h };
    if (texMat) {
        SDL_RenderCopy(renderer, texMat, NULL, &destMat);
    } else {
        // Si on as pas de texture pour le mât, on dessine juste un poteau gris !
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderFillRect(renderer, &destMat);
    }

    // 2. On affiche le drapeau par-dessus
    if (texDrapeau) {
        SDL_Rect destFlag = { f->flagRect.x - camX, f->flagRect.y - camY, f->flagRect.w, f->flagRect.h };
        SDL_RenderCopy(renderer, texDrapeau, NULL, &destFlag);
    }
}