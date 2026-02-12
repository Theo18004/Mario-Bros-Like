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
    SDL_Rect rect;
    float velocityY;
    int isJumping;
    SDL_Texture* texIdle;
    SDL_Texture* texRun;
    SDL_Texture* texJump;
    AnimState animState;
    int facingLeft;
    float frameTimer;
} Player;

void updatePlayer(Player* player, int* tileMap);
void avancer(Player* player, int speed);
void saut(Player* player);
void descendre(Player* player);

#endif