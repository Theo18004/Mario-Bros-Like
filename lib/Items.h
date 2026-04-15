/**
 * @file Items.h
 * @brief Gestion des objets ramassables du jeu (Pièces).
 */

#ifndef ITEMS_H
#define ITEMS_H
#include <SDL.h>

/**
 * @struct Piece
 * @brief Représente un objet ramassable (pièce) sur la carte.
 */
typedef struct {
    SDL_Rect rect; /**< Position de la pièce */
    int vivant;    /**< Indique si la pièce est vivante (1) ou non (0) */
} Piece;

/** @def NB_PIECES
 * @brief Nombre total de pièces présentes dans le niveau.
 */
#define NB_PIECES 4

/**
 * @brief Affiche une pièce à l'écran.
 * @param texCoin La texture contenant le dessin de la pièce.
 * @param x Position X de la pièce dans le monde.
 * @param y Position Y de la pièce dans le monde.
 */
void render_coin(SDL_Renderer* renderer, SDL_Texture* texCoin, int x, int y, int camX, int camY);

/** @def NB_CHECKPOINTS
 * @brief Nombre total de Checkpoints
 */
#define NB_CHECKPOINTS 3

/**
 * @struct Checkpoint
 * @brief Représente les points de sauvegarde de progression dans le niveau
 */
typedef struct {
    SDL_Rect rect; /**< Position du checkpoint */
    int actif;  /**< Indique si le checkpoint est actif (1) ou non (0) */
    Uint32 animStart;    /**< Temps de début de l'animation */
} Checkpoint;

/**
 * @brief Dessine l'animation d'ouverture de la fleur Checkpoint.
 * @param renderer Le renderer du SDL.
 * @param texDrapeau Texture représentant le tissu.
 * @param d Pointeur vers le Checkpoint.
 * @param camX Décalage de la caméra en X.
 * @param camY Décalage de la caméra en Y.
 */
// Déclaration de la fonction pour l'affichage des drapeau 
void render_checkpoint(SDL_Renderer* renderer, SDL_Texture* texDrapeau, Checkpoint* d, int camX, int camY);


#endif