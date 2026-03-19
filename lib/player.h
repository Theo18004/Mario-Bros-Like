/**
 * @file player.h
 * @brief Gestion du joueur (états, physique et rendu).
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "score.h"


/** @enum PlayerState */
typedef enum {
    STATE_IDLE, /**< Au repos */
    STATE_RUN,  /**< En train de courir */
    STATE_JUMP, /**< Dans les airs */
    STATE_DEAD  /**< En cours d'animation de mort */
} PlayerState;

/**
 * @struct Player
 * @brief Structure regroupant les données du personnage principal.
 */
typedef struct {
    SDL_Rect rect;      /**< Hitbox et position */
    float velY;         /**< Vélocité verticale */
    int facingRight;    /**< Orientation du regard */
    int onGround;       /**< Est au sol ? */
    int lives;          /**< Nombre de vies restantes */
    PlayerState state;  /**< État actuel pour les animations */
    int checkpointX;
    int checkpointY;
} Player;

void init_player(Player* p, int x, int y);
void update_player(Player* p, const Uint8* keys, int* map);
void render_player(SDL_Renderer* renderer, Player* p, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun, SDL_Texture* texJump, SDL_Texture* texDead);

#endif