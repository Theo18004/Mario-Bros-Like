/**
 * @file player.h
 * @brief Gestion du joueur (états, physique et rendu).
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "interface.h"


/** @enum PlayerState */
typedef enum {
    STATE_IDLE, /**< Au repos */
    STATE_RUN,  /**< En train de courir */
    STATE_JUMP, /**< Dans les airs */
    STATE_DEAD, /**< En cours d'animation de mort */
    STATE_WIN  /**< A gagné */
} PlayerState;

/**
 * @struct Player
 * @brief Structure regroupant les données du personnage principal.
 */
typedef struct Player{
    SDL_Rect rect;      /**< Hitbox et position */
    float velY;         /**< Vélocité verticale */
    float velX;         /**< Vélocité horizontale */
    int facingRight;    /**< Orientation du regard */
    int onGround;       /**< Est au sol ? */
    int lives;          /**< Nombre de vies restantes */
    PlayerState state;  /**< État actuel pour les animations */
    int checkpointX;
    int checkpointY;
} Player;

/**
 * @brief Initialise le joueur.
 * @param p Pointeur vers le joueur.
 * @param x Position X initiale.
 * @param y Position Y initiale.
 */
void init_player(Player* p, int x, int y);

/**
 * @brief Met à jour l'état du joueur.
 * @param p Pointeur vers le joueur.
 * @param keys État des touches (clavier).
 * @param map Carte du niveau.
 * @param levelID ID du niveau actuel.
 */
void update_player(Player* p, const Uint8* keys, int* map, int levelID);

/**
 * @brief Affiche le joueur à l'écran.
 * @param renderer Le renderer SDL.
 * @param p Pointeur vers le joueur.
 * @param scrollX Décalage de la caméra en X.
 * @param scrollY Décalage de la caméra en Y.
 * @param texIdle Texture du joueur au repos.
 * @param texRun Texture du joueur en course.
 * @param texJump Texture du joueur en saut.
 * @param texDead Texture du joueur mort.
 */
void render_player(SDL_Renderer* renderer, Player* p, int scrollX, int scrollY,
                   SDL_Texture* texIdle, SDL_Texture* texRun, SDL_Texture* texJump, SDL_Texture* texDead);

#endif
