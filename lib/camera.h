#ifndef CAMERA_H
#define CAMERA_H

#include "player.h" // Pour connaître la pos du joueur

typedef struct {
    int x;
    int y;
    int w; // Largeur logique de l'écran
    int h; // Hauteur logique
} Camera;

// Calcule la position de la caméra en fonction du joueur
void update_camera(Camera* cam, Player* p, int mapWidthPixel, int mapHeightPixel);

#endif