/**
 * @file defs.h
 * @brief Constantes globales et réglages du moteur de jeu.
 */

#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *sonSaut;
extern Mix_Chunk *bouleFeu;
extern Mix_Chunk *coin;
extern Mix_Chunk *sonThwomp;

#define TILE_SIZE 16        /**< Taille de base d'une tuile (pixels) */
#define MAP_SCALE 2         /**< Facteur d'agrandissement de l'affichage */
#define MAP_WIDTH 484       /**< Nombre de tuiles en largeur */
#define MAP_HEIGHT 38       /**< Nombre de tuiles en hauteur */
#define REFERENCE_HEIGHT 750 /**< Hauteur de référence pour la mise à l'échelle */
#define TILE_SOLID 7        /**< Identifiant d'une tuile solide générique */

#endif