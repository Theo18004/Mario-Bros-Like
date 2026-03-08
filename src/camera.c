#include "camera.h"
#include "defs.h"

void update_camera(Camera* cam, Player* p, int mapWidthPixel, int mapHeightPixel) {

    // Centrer sur le joueur
    cam->x = p->rect.x - cam->w / 2;

    // Bloquer à gauche
    if (cam->x < 0) cam->x = 0;
    
    // Bloquer à droite
    int maxScrollX = mapWidthPixel - cam->w;
    if (cam->x > maxScrollX) cam->x = maxScrollX;

    // Camera de base fixé au sol
    int base_y = mapHeightPixel - cam->h;
    
    // Décalage vertical 
    int decallage_hauteur = 100; // Ajuste cette valeur pour changer la hauteur à laquelle la caméra commence à suivre le joueur
    int camera_decalee_y = p->rect.y - decallage_hauteur; 

    //Si le joueur est plus haut que la base, on monte la camera
    if (camera_decalee_y < base_y) {
        cam->y = camera_decalee_y; 
    } else {
        cam->y = base_y;
    }
    
    // 4. Bloquer tout en haut pour ne pas sortir de la carte
    if (cam->y < 0) cam->y = 0;

}