#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>

#define TILE_SIZE 16
#define MAP_SCALE 2
#define MAP_WIDTH 192
#define MAP_HEIGHT 32

typedef struct {
    SDL_Texture* texture;
    int columns;
    int tileWidth;
    int tileHeight;
} Tileset;

// Fonction pour charger les données CSV dans un tableau
int load_map_from_csv(const char* filename, int* map_array);

// Fonction pour dessiner une tuile spécifique
void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY);

// Fonction pour dessiner toute la carte d'un coup
void render_world(SDL_Renderer* renderer, int* map_array, Tileset* ts, int scrollX, int scrollY);

#endif