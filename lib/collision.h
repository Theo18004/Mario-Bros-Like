#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>
#include "player.h"

// Vérifie si une tuile spécifique est solide (6, 7, 8 ou 28+)
int is_solid(int tileId);

// Gère la collision horizontale avec la TileMap
void HorizontaleCollision(Player* player, int* tileMap, int moveX);

// Gère la collision verticale (sol/plafond) avec la TileMap
void VerticaleCollision(Player* player, int* tileMap);

#endif