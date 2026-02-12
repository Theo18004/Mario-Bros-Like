#include "camera.h"

/*
* Met à jour la position de la caméra pour suivre une cible (le joueur). 
* La caméra est centrée sur le joueur, mais on s'assure qu'elle ne sort pas des limites de la map.
*/


void updateCamera(Camera* cam, SDL_Rect target, int screenW, int screenH) {
    cam->x = (target.x + target.w / 2) - (screenW / 2);
    cam->y = (target.y + target.h / 2) - (screenH / 2);
    if (cam->x < 0) cam->x = 0;
    if (cam->y > 0) cam->y = 0; 
}