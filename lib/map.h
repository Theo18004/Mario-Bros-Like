/**
 * @file map.h
 * @brief Gestion de la Tilemap (carte de tuiles) et des décors.
 */

#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "defs.h"

/**
 * @struct Tileset
 * @brief Représente une planche de tuiles (spritesheet de décors).
 */
typedef struct {
    SDL_Texture* texture; /**< La texture contenant toutes les tuiles */
    int columns;          /**< Nombre de tuiles par ligne sur l'image */
    int tileWidth;        /**< Largeur d'une tuile (en pixels) */
    int tileHeight;       /**< Hauteur d'une tuile (en pixels) */
} Tileset;

/**
 * @brief Charge les IDs des tuiles d'un niveau depuis un fichier CSV.
 * @param filename Chemin vers le fichier CSV.
 * @param map_array Tableau d'entiers qui sera rempli avec les données.
 */
int load_map_from_csv(const char* filename, int* map_array);

/**
 * @brief Dessine une tuile à l'écran.
 * @param renderer Le renderer SDL.
 * @param ts Le tileset contenant la tuile.
 * @param tileId L'ID de la tuile à dessiner.
 * @param x, y Position absolue de la tuile dans le monde.
 * @param scrollX, scrollY Décalage de la caméra.
 */
void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY);

/**
 * @brief Affiche un arrière-plan avec effet de défilement (Parallaxe).
 * @param renderer Le renderer SDL.
 * @param tex La texture de l'arrière-plan.
 * @param scrollX, scrollY Décalage de la caméra.
 * @param speedX, speedY Vitesse de défilement relative à la caméra.
 * @param screenW, screenH Dimensions de l'écran.
 * @param startX, offsetY Position d'origine de l'image de fond.
 */
void draw_parallax_bg(SDL_Renderer* renderer, SDL_Texture* tex, int scrollX, int scrollY, float speedX, float speedY, int screenW, int screenH, int startX, int offsetY);

#endif