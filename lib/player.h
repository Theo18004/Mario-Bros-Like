#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

// Physique
#define GRAVITY 0.5f
#define JUMP_FORCE -12.0f
#define GROUND_Y 540

// Animation
typedef enum {
    ANIM_IDLE,
    ANIM_RUN,
    ANIM_JUMP
} AnimState;

typedef struct {
    SDL_Rect rect;      // Hitbox (collision)
    float velocityY;    // Physique verticale
    int isJumping;      // Physique saut
    
    // NOUVEAU : Gestion visuelle
    SDL_Texture* texIdle;
    SDL_Texture* texRun;
    SDL_Texture* texJump;
    
    AnimState animState; // IDLE, RUN ou JUMP
    int facingLeft;      // 1 = Gauche, 0 = Droite
    int frameIndex;      // Quelle image du sprite afficher (0, 1, 2...)
    float frameTimer;    // Chrono pour changer d'image
} Player;

void updatePlayer(Player* player);
void avancer(Player* player, int speed);
void saut(Player* player);
void descendre(Player* player);

#endif