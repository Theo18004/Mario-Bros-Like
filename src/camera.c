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

    // (Pour l'instant le scrollY est fixe en bas dans ton jeu, 
    // mais on pourrait l'animer ici aussi si besoin)
    cam->y = mapHeightPixel - cam->h;
}