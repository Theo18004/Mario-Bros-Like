#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>
#include "defs.h"

// Renvoie 1 si collision avec un mur, 0 sinon
int check_collision(SDL_Rect rect, int* map);

// Renvoie 1 si le tile est un un bloc solide, 0 sinon
int is_solid(int tile_id);

#endif