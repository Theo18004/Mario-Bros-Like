#ifndef COLLISION_H
#define COLLISION_H


#include <player.h>
#include <SDL2/SDL.h>


int collision( SDL_Rect a, SDL_Rect b);

void HorizontaleCollision (Player * player, SDL_Rect * plateforms, int platformCount, int moveX);

void VerticaleCollision (Player * player, SDL_Rect * plateforms, int platformCount);

#endif