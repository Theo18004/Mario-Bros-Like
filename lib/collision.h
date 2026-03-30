/**
 * @file collision.h
 * @brief Détection de collisions avec les tuiles de la carte (blocs et pentes).
 */

#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>
#include "defs.h"

/**
 * @brief Vérifie si un rectangle entre en collision avec des blocs solides.
 * @param rect Le rectangle à tester (hitbox).
 * @param map Le tableau représentant la carte.
 * @param check_demi Indique les demi-plateformes 
 * @return 1 si collision, 0 sinon.
 */
int check_collision(SDL_Rect rect, int* map, int check_demi, int levelID);

/** @brief Vérifie si une tuile est un bloc plein. */
int is_full(int tile_id, int levelID);

/** @brief Vérifie si une tuile est une pente montante vers la droite. */
int is_slope_up_right(int tile_id, int levelID);

/** @brief Vérifie si une tuile est une pente montante vers la gauche. */
int is_slope_up_left(int tile_id, int levelID);

/** @brief Calcule la hauteur exacte sur une pente. */
int get_slope(int worldX, int tileX, int tileY, int tile_id, int levelID);

#endif