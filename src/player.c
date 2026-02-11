#include "player.h"



/** 
    * Avance ou recule le joueur en fonction des touches fléchées.
    * @param player : pointeur vers la structure du joueur
    * @param speed : vitesse de déplacement (ex: 5 pixels par frame)
    * Note : Cette fonction gère aussi la direction du regard du joueur (facingLeft)
    * Exemple d'utilisation : avancer(&player, 5);
    *   - Si la touche flèche droite est pressée, le joueur avance vers la droite et regarde à droite.
    *  - Si la touche flèche gauche est pressée, le joueur recule vers la gauche et regarde à gauche.
    *  - Si aucune touche n'est pressée, le joueur ne bouge pas et garde sa direction actuelle.
    * Note : Cette fonction doit être appelée à chaque frame pour que le mouvement soit fluide.
    * Note : La vitesse peut être ajustée pour rendre le jeu plus rapide ou plus lent.
    * 

*/
void avancer(Player* player, int speed) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_RIGHT]) {
        player->rect.x += speed;
        player->facingLeft = 0; // Regarde à droite
    }
    if (state[SDL_SCANCODE_LEFT]) {
        player->rect.x -= speed;
        player->facingLeft = 1; // Regarde à gauche
    }
}
/** 
    * Permet au joueur de sauter lorsqu'on appuie sur la touche flèche haut.
    * @param player : pointeur vers la structure du joueur
    * Note : Le joueur ne peut sauter que s'il n'est pas déjà en train de sauter (isJumping == 0).
    * Note : Lorsqu'on appuie sur la touche flèche haut, on applique une force de saut (JUMP_FORCE) à la vitesse verticale du joueur.
    * Note : La gravité (GRAVITY) sera appliquée dans la fonction updatePlayer pour faire retomber le joueur après le saut.
    * Exemple d'utilisation : saut(&player);
    * - Si la touche flèche haut est pressée et que le joueur n'est pas déjà en train de sauter, le joueur commence à sauter.
    * - Si la touche flèche haut est pressée mais que le joueur est déjà en train de sauter, rien ne se passe (pas de double saut).
    * Note : Cette fonction doit être appelée à chaque frame pour que le saut soit réactif.
*/
void saut(Player* player) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP] && !player->isJumping) {
        player->velocityY = JUMP_FORCE;
        player->isJumping = 1;
    }
}


/** 
    * Permet au joueur de descendre plus rapidement lorsqu'on appuie sur la touche flèche bas.
    * @param player : pointeur vers la structure du joueur
    * Note : Cette fonction ajoute une force de gravité supplémentaire lorsque la touche flèche bas est pressée, ce qui fait que le joueur descend plus rapidement.
    * Note : Cela peut être utile pour les jeux de plateforme où le joueur veut descendre rapidement d'une plateforme ou éviter un obstacle en sautant.
    * Exemple d'utilisation : descendre(&player);
    * - Si la touche flèche bas est pressée, le joueur descend plus rapidement.
    * - Si la touche flèche bas n'est pas pressée, le joueur descend à la vitesse normale définie par GRAVITY.
    * Note : Cette fonction doit être appelée à chaque frame pour que l'effet de descente rapide soit réactif.
*/

void descendre(Player* player) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_DOWN]) {
        player->velocityY += GRAVITY;
    }
}

/** 
    * Met à jour la position, la physique et l'animation du joueur à chaque frame.
    * @param player : pointeur vers la structure du joueur
    * Note : Cette fonction doit être appelée à chaque frame pour que le jeu fonctionne correctement.
    * Note : La fonction gère les inputs (avancer, sauter, descendre), applique la physique (gravité) et met à jour l'état de l'animation en fonction de ce que fait le joueur.
    * Exemple d'utilisation : updatePlayer(&player);
    * - Si le joueur appuie sur les touches fléchées, il se déplace, saute ou descend plus rapidement.
    * - La gravité est appliquée pour faire retomber le joueur après un saut.
    * - L'état de l'animation est mis à jour pour refléter si le joueur est en train de courir, sauter ou rester immobile.
    * Note : Cette fonction est le cœur de la logique du joueur et doit être appelée à chaque frame pour que le jeu soit fluide et réactif.
    * Note : La gestion de l'animation est basique pour l'instant, mais elle peut être améliorée en ajoutant des sprites et en gérant les différentes frames d'animation.
    * Note : La gestion des collisions avec les plateformes n'est pas encore implémentée dans cette fonction, elle sera ajoutée plus tard.
*/




void updatePlayer(Player* player) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    // 1. Gestion des inputs
    // On vérifie si on bouge pour l'état "Run"
    int isMoving = (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_LEFT]);
    
    avancer(player, 5);
    saut(player);
    descendre(player);

    // 2. Gestion de la physique (gravité)
    player->rect.y += (int)player->velocityY;
    player->velocityY += GRAVITY;

    if (player->rect.y >= GROUND_Y) {
        player->rect.y = GROUND_Y;
        player->velocityY = 0;
        player->isJumping = 0;
    }

    // 3. GESTION DE L'ANIMATION
    // Quelle animation doit-on jouer ?
    if (player->isJumping) {
        player->animState = ANIM_JUMP;
    } else if (isMoving) {
        player->animState = ANIM_RUN;
    } else {
        player->animState = ANIM_IDLE;
    }

    // On fait avancer le chronomètre de l'animation
    // 0.2f est la vitesse de l'animation (plus c'est haut, plus ça va vite)
    player->frameTimer += 0.2f; 
}