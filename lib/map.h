/**
 * @file map.h
 * @brief Gestion de la carte, du chargement CSV et du parallaxe.
 */

#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include "defs.h"

/**
 * @struct Tileset
 * @brief Stocke la texture et les dimensions des tuiles.
 */
typedef struct {
    SDL_Texture* texture; /**< Feuille de sprites des tuiles */
    int columns;          /**< Nombre de colonnes dans l'image */
    int tileWidth;        /**< Largeur d'une tuile source */
    int tileHeight;       /**< Hauteur d'une tuile source */
} Tileset;

int load_map_from_csv(const char* filename, int* map_array);
void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY);
void draw_parallax_bg(SDL_Renderer* renderer, SDL_Texture* tex, int scrollX, int scrollY, float speedX, float speedY, int screenW, int screenH, int offsetY);

#endif