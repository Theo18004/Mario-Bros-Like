#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "defs.h"

typedef struct {
    SDL_Texture* texture;
    int columns;          
    int tileWidth;
    int tileHeight;
} Tileset;

// Charge la map depuis le CSV
int load_map_from_csv(const char* filename, int* map_array);

// Dessine une tuile unique
void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY);

// Dessine les arrières-plans (Parallaxe)
void draw_parallax_bg(SDL_Renderer* renderer, SDL_Texture* tex, int scrollX, int scrollY, float speedX, float speedY, int screenW, int screenH, int offsetY);

#endif