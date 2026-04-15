/**
 * @file defs.h
 * @brief Constantes globales et réglages du moteur de jeu.
 */

#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *sonSaut; /**< Son du saut */
extern Mix_Chunk *bouleFeu; /**< Son de la boule de feu */
extern Mix_Chunk *coin; /**< Son de la pièce */
extern Mix_Chunk *sonThwomp; /**< Son du Thwomp */
extern Mix_Chunk *sonJC; /**< Son de JC */
extern Mix_Chunk *sonEcrasement; /**< Son de l'écrasement */
extern Mix_Chunk *sonMortJoueur; /**< Son de la mort du joueur */
extern Mix_Chunk *sonLoup; /**< Son du Loup */
extern Mix_Chunk *sonAlien; /**< Son de l'Alien */
extern Mix_Chunk *sonDefaite; /**< Son de la défaite */

#define TILE_SIZE 16        /**< Taille de base d'une tuile (pixels) */
#define MAP_SCALE 2         /**< Facteur d'agrandissement de l'affichage */
#define MAP_WIDTH 525       /**< Nombre de tuiles en largeur */
#define MAP_HEIGHT 38       /**< Nombre de tuiles en hauteur */
#define REFERENCE_HEIGHT 750 /**< Hauteur de référence pour la mise à l'échelle */
#define TILE_SOLID 7        /**< Identifiant d'une tuile solide générique */

#endif