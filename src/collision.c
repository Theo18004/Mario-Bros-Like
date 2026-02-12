#include <collision.h>




/*
    

*/



// Vérifie si deux rectangles se touchent
int collision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

/*
    * Gère les collisions horizontales du joueur avec les plateformes. 
    * @param player : pointeur vers la structure du joueur 
    * @param platforms : tableau de rectangles représentant les plateformes 
    * @param platformCount : nombre de plateformes dans le tableau 
    * @param moveX : déplacement horizontal que le joueur souhaite effectuer (ex: -5 pour aller à gauche, 5 pour aller à droite, 0 pour ne pas bouger) 
    * Note : Cette fonction doit être appelée avant d'appliquer le mouvement horizontal au joueur. * Note : Si une collision est détectée après avoir appliqué moveX, la position horizontale du joueur est ajustée pour qu'il ne traverse pas la plateforme. 
    * Exemple d'utilisation : HorizontaleCollision(&player, worldPlatforms, platformCount, moveX); 
    * - Si le joueur essaie de se déplacer vers une plateforme, il s'arrête juste avant de la toucher. 
    * - Si le joueur essaie de se déplacer à travers une plateforme, il est repositionné pour rester à l'extérieur de celle-ci. 
    * Note : Cette fonction gère uniquement les collisions horizontales. Les collisions verticales sont gérées séparément dans VerticaleCollision. 
*/


void HorizontaleCollision(Player* player, SDL_Rect* platforms, int platformCount, int moveX) {
    if (moveX == 0) return;

    // On applique le mouvement
    player->rect.x += moveX;

    // On vérifie si ce mouvement crée une collision
    for (int i = 0; i < platformCount; i++) {
        if (collision(player->rect, platforms[i])) {
            if (moveX > 0) // Vers la droite
                player->rect.x = platforms[i].x - player->rect.w;
            else if (moveX < 0) // Vers la gauche
                player->rect.x = platforms[i].x + platforms[i].w;
        }
    }
}



/*
    * Gère les collisions verticales du joueur avec les plateformes. 
    * @param player : pointeur vers la structure du joueur 
    * @param platforms : tableau de rectangles représentant les plateformes 
    * @param platformCount : nombre de plateformes dans le tableau 
    * Note : Cette fonction doit être appelée après avoir appliqué la gravité à player->velocityY et mis à jour player->rect.y en conséquence. 
    * Note : Si une collision est détectée, la position verticale du joueur est ajustée pour qu'il ne traverse pas la plateforme, et la vitesse verticale est réinitialisée à 0. * Note : Si le joueur retombe sur une plateforme, isJumping est mis à 0 pour permettre un nouveau saut. 
    * Exemple d'utilisation : VerticaleCollision(&player, worldPlatforms, platformCount); 
    * - Si le joueur tombe sur une plateforme, il s'arrête dessus et peut sauter à nouveau. 
    * - Si le joueur cogne le plafond d'une plateforme en sautant, il s'arrête et commence à retomber. 
    * Note : Cette fonction gère uniquement les collisions verticales. Les collisions horizontales sont gérées séparément dans HorizontaleCollision. 
*/

void VerticaleCollision(Player* player, SDL_Rect* platforms, int platformCount) {
    // On applique la vitesse verticale (saut ou gravité)
    player->rect.y += (int)player->velocityY;

    for (int i = 0; i < platformCount; i++) {
        if (collision(player->rect, platforms[i])) {
            if (player->velocityY > 0) { // Retombe sur une plateforme
                player->rect.y = platforms[i].y - player->rect.h;
                player->velocityY = 0;
                player->isJumping = 0;
            }
            else if (player->velocityY < 0) { // Cogne le plafond
                player->rect.y = platforms[i].y + platforms[i].h;
                player->velocityY = 0;
            }
        }
    }
}