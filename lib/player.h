#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

typedef enum {
    STATE_IDLE,
    STATE_RUN,
    STATE_JUMP,
    STATE_DEAD
} PlayerState;

// Structure regroupant tout ce qui concerne le joueur
typedef struct {
    SDL_Rect rect;      // Hitbox physique (x, y, w, h)
    float velY;         // Vitesse verticale
    int facingRight;    // 1 = Droite, 0 = Gauche
    int onGround;       // 1 = Sol, 0 = Air
    PlayerState state;  // État actuel
} Player;

// Initialise le joueur
void init_player(Player* p, int x, int y);

// Met à jour la position et l'état (Physique + Inputs)
void update_player(Player* p, const Uint8* keys, int* map);

// Dessine le joueur avec l'animation correcte
void render_player(SDL_Renderer* renderer, Player* p, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun, SDL_Texture* texJump, SDL_Texture* texDead);

#endif