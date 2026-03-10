/**
 * @file loupas.h
 * @brief Gestion des entités ennemies (Loup).
 */

#ifndef ENNEMI_H
#define ENNEMI_H

#include <SDL.h>
#include "player.h"

/**
 * @struct Ennemi
 * @brief Données d'un ennemi Loup patrouilleur.
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

void init_loupas(Ennemi* e, int x, int y);
void update_loupas(Ennemi* e, int* map);

/**
 * @brief Affiche l'ennemi Loup en animant sa Sprite Sheet.
 * @param renderer Le renderer SDL.
 * @param e Pointeur vers l'ennemi.
 * @param scrollX Décalage caméra X.
 * @param scrollY Décalage caméra Y.
 * @param texEnnemi La sprite sheet du loup (wolfsheet.png).
 */
void render_loupas(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, 
                   SDL_Texture* texEnnemi);

#endif