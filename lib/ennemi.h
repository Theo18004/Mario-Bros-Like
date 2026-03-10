/**
 * @file ennemi.h
 * @brief Gestion des entités ennemies et de leur IA.
 */

#ifndef ENNEMI_H
#define ENNEMI_H

#include <SDL.h>
#include "player.h"

/**
 * @struct Ennemi
 * @brief Données d'un ennemi patrouilleur.
 */
typedef struct {
    SDL_Rect rect;      /**< Hitbox de l'ennemi */
    float velY;         /**< Vélocité verticale */
    float speed;        /**< Vitesse de marche */
    int direction;      /**< 1 pour droite, -1 pour gauche */
    int onGround;       /**< État au sol */
    PlayerState state;  /**< État d'animation */
    int vivant;         /**< 1 si actif, 0 si supprimé */
} Ennemi;

void init_ennemi(Ennemi* e, int x, int y);
void update_ennemi(Ennemi* e, int* map);
void render_ennemi(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun);

#endif