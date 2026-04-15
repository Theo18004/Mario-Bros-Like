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

/** 
 * @brief Vérifie si une tuile est un bloc plein. 
 * @param tile_id L'identifiant de la tuile.
 * @param levelID L'identifiant du niveau.
 * @return 1 si la tuile est pleine, 0 sinon.
 */
int is_full(int tile_id, int levelID);

/** 
 * @brief Vérifie si une tuile est une pente montante vers la droite. 
 * @param tile_id L'identifiant de la tuile.
 * @param levelID L'identifiant du niveau.
 * @return 1 si la tuile est une pente montante vers la droite, 0 sinon.
 */
int is_slope_up_right(int tile_id, int levelID);

/** @brief Vérifie si une tuile est une pente montante vers la gauche. 
 * @param tile_id L'identifiant de la tuile.
 * @param levelID L'identifiant du niveau.
 * @return 1 si la tuile est une pente montante vers la gauche, 0 sinon.
 */
int is_slope_up_left(int tile_id, int levelID);

/** @brief Vérifie si le joueur est sur un bloc de glace. 
 * @param rect Le rectangle représentant le joueur.
 * @param map La carte du niveau.
 * @param levelID L'identifiant du niveau.
 * @return 1 si le joueur est sur un bloc de glace, 0 sinon.
 */
int check_on_ice(SDL_Rect rect, int* map, int levelID);

/** @brief Calcule la hauteur exacte sur une pente. 
 * @param worldX La position X dans le monde.
 * @param tileX La position X de la tuile.
 * @param tileY La position Y de la tuile.
 * @param tile_id L'identifiant de la tuile.
 * @param levelID L'identifiant du niveau.
 * @return La hauteur exacte sur la pente.
 */
int get_slope(int worldX, int tileX, int tileY, int tile_id, int levelID);

#endif