/**
 * @file camera.c
 * @brief Logique de calcul du défilement de la vue.
 */
#include "camera.h"
#include "defs.h"

void update_camera(Camera* cam, Player* p, int mapWidthPixel, int mapHeightPixel) {
    // Centrer sur le joueur
    cam->x = p->rect.x - cam->w / 2;

    // Bloquer les bords horizontaux
    if (cam->x < 0) cam->x = 0;
    int maxScrollX = mapWidthPixel - cam->w;
    if (cam->x > maxScrollX) cam->x = maxScrollX;

    // Camera de base fixé au sol
    int base_y = mapHeightPixel - cam->h;
    
    // Suivi vertical avec décalage
    int decallage_hauteur = 100; 
    int camera_decalee_y = p->rect.y - decallage_hauteur; 

    if (camera_decalee_y < base_y) {
        cam->y = camera_decalee_y; 
    } else {
        cam->y = base_y;
    }
    
    if (cam->y < 0) cam->y = 0;
}