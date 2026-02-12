#include "camera.h"

void updateCamera(Camera* cam, SDL_Rect target, int screenW, int screenH) {
    // On centre la caméra sur le milieu de la cible (le joueur)
    cam->x = (target.x + target.w / 2) - (screenW / 2);
    cam->y = (target.y + target.h / 2) - (screenH / 2);

    // Limite gauche : la caméra ne doit pas aller en dessous de 0
    if (cam->x < 0) cam->x = 0;
    
    // Si tu connais la largeur totale de ta map (ex: 5000px), 
    // tu peux aussi bloquer la caméra à droite :
    // if (cam->x > 5000 - screenW) cam->x = 5000 - screenW;

    // Pour un Mario-like classique, on bloque souvent la caméra en Y à 0
    if (cam->y > 0) cam->y = 0; 
}