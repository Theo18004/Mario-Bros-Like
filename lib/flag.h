/**
 * @file flag.h
 * @brief Gestion du joueur (états, physique et rendu).
 */

#ifndef FLAG_H
#define FLAG_H

#include <SDL2/SDL.h>

// --- Structure Flag ---
/**
 * @struct Flag
 * @brief Représente le drapeau de fin de niveau déclenchant la victoire.
 */
typedef struct Flag{
    SDL_Rect matRect;   /**< Hitbox représentant le mât du drapeau */
    SDL_Rect flagRect;  /**< Hitbox représentant le tissu du drapeau */
    int actif;          /**< Etat de l'animation 0 = en haut, 1 = descend, 2 = en bas */
} Flag;

// --- Prototypes des fonctions ---

/**
 * @brief Initialise les paramètres de départ du drapeau.
 * @param f Pointeur vers l'entité Flag.
 * @param x Position X initiale du mât.
 * @param y Position Y initiale du bas du mât.
 */
void init_flag(Flag* f, int x, int y);

/**
 * @brief Met à jour l'animation de descente du drapeau.
 * @param f Pointeur vers l'entité Flag.
 */
void update_flag(Flag* f); // dt est le temps écoulé (delta time)

/**
 * @brief Dessine le drapeau et son mât à l'écran.
 * @param renderer Le renderer du SDL.
 * @param f Pointeur vers l'entité Flag.
 * @param texMat Texture représentant le mât.
 * @param texDrapeau Texture représentant le tissu.
 * @param camX Décalage de la caméra en X.
 * @param camY Décalage de la caméra en Y.
 */
void render_flag(SDL_Renderer* renderer, Flag* f, SDL_Texture* texMat,
     SDL_Texture* texDrapeau, int camX, int camY);

#endif