#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Camera;

// Met à jour la position de la caméra pour suivre une cible (le joueur)
void updateCamera(Camera* cam, SDL_Rect target, int screenW, int screenH);

#endif