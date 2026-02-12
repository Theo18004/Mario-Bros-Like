/**
 * @file ennemi.h
 * @brief Définitions et prototypes pour la gestion des ennemis (Singes) en SDL2.
 */

#ifndef MONKEY_H
#define MONKEY_H

#include <SDL2/SDL.h>
#include "camera.h"

/** @name Configuration du Sprite Sheet */
/**@{*/
#define MONKEY_COLS 4          /**< Colonnes dans la grille de sprites */
#define MONKEY_ROWS 4          /**< Lignes dans la grille de sprites */
#define MONKEY_TOTAL_FRAMES 16 /**< Total d'images d'animation */
/**@}*/

/**
 * @struct Monkey
 * @brief Structure représentant un ennemi de type singe (Version SDL2).
 */
typedef struct {
    SDL_Point position;  /**< Position (x, y) dans le monde global */
    SDL_Point size;      /**< Taille d'une frame d'animation */
    float speed;         /**< Vitesse de déplacement */
    int direction;       /**< Direction : 1 = Droite, -1 = Gauche */
    
    int currentFrame;    /**< Index de l'image (0-15) */
    float frameTimer;    /**< Chronomètre d'animation */
    float animFPS;       /**< Vitesse d'animation */
    
    SDL_Texture* spriteSheet; /**< Texture SDL2 chargée */
} Monkey;

/**
 * @brief Initialise un singe avec une texture SDL2.
 */
Monkey InitMonkey(SDL_Texture* texture, SDL_Point pos, float speed);

/**
 * @brief Met à jour la position et l'animation du singe.
 */
void UpdateMonkey(Monkey *m, float deltaTime);

/**
 * @brief Affiche le singe en tenant compte de la caméra.
 */
void DrawMonkey(SDL_Renderer* renderer, Monkey* m, Camera cam);

#endif