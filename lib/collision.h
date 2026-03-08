#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>
#include "defs.h"

// Vérifie si le rectangle donné entre en collision avec des blocs solides sur la map
int check_collision(SDL_Rect rect, int* map);

//Renvoie 1 si la tuile est un bloc plein, 0 sinon
int is_full(int tile_id);

// Renvoie 1 si la tuile est un escalier vers la droite, 0 sinon
int is_slope_up_right(int tile_id);

// Renvoie 1 si la tuile est un escalier vers la gauche, 0 sinon
int is_slope_up_left(int tile_id);

// Renvoie 1 si la tuile est un escalier, 0 sinon
int get_slope(int worldX, int tileX, int tileY, int tile_id);



#endif