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
    SDL_Rect rect;
    int vivant; 
} Piece;

/** @def NB_PIECES
 * @brief Nombre total de pièces présentes dans le niveau.
 */
#define NB_PIECES 5

/**
 * @brief Affiche une pièce à l'écran.
 * @param texCoin La texture contenant le dessin de la pièce.
 * @param x Position X de la pièce dans le monde.
 * @param y Position Y de la pièce dans le monde.
 */
void render_coin(SDL_Renderer* renderer, SDL_Texture* texCoin, int x, int y, int camX, int camY);

#define NB_CHECKPOINTS 3

typedef struct {
    SDL_Rect rect; 
    int actif;  
    Uint32 animStart;    
} Checkpoint;

// Déclaration de la fonction pour l'afficher
void render_checkpoint(SDL_Renderer* renderer, SDL_Texture* texDrapeau, Checkpoint* d, int camX, int camY);


#endif