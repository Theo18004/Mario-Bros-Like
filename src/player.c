#include "player.h"
#include "collision.h"

/*
* Gère le déplacement horizontal du joueur en fonction de l'input clavier. 
* On utilise la collision horizontale pour valider le mouvement, c'est là qu'on vérifie si on fonce dans une tuile solide (6, 7 ou 8). 
* Si une collision est détectée, le joueur est ajusté pour "coller" au mur et ne pas passer à travers.
*/

void avancer(Player* player, int speed) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    int moveX = 0;

    if (state[SDL_SCANCODE_RIGHT]) {
        moveX = speed;
        player->facingLeft = 0;
        player->animState = ANIM_RUN;
    } else if (state[SDL_SCANCODE_LEFT]) {
        moveX = -speed;
        player->facingLeft = 1;
        player->animState = ANIM_RUN;
    } else {
        player->animState = ANIM_IDLE;
    }

    // On utilise la collision horizontale pour valider le mouvement
    // C'est ici qu'on vérifie si on fonce dans une tuile 6, 7 ou 8
    HorizontaleCollision(player, (int*)player, moveX); 
}

/*
    * Gère le saut du joueur. 
    * Si la touche UP est pressée et que le joueur n'est pas déjà en train de sauter, on applique une force de saut (velocityY) et on change l'état d'animation. 
    * La variable isJumping empêche de faire des doubles sauts.
*/

void saut(Player* player) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP] && !player->isJumping) {
        player->velocityY = JUMP_FORCE;
        player->isJumping = 1;
        player->animState = ANIM_JUMP;
    }
}

/*
    * Gère la gravité qui fait descendre le joueur. 
    * À chaque frame, on ajoute une valeur de gravité à velocityY, ce qui fait que le joueur accélère vers le bas. 
    * La collision verticale avec la tileMap (VerticaleCollision) s'assure que le joueur ne tombe pas à travers le sol.
*/
void descendre(Player* player) {
    // La gravité fait descendre le joueur
    player->velocityY += GRAVITY;
}

// --- LA FONCTION PRINCIPALE MISE À JOUR ---


/*
* Gère la mise à jour complète du joueur à chaque frame. 
* On gère d'abord le déplacement horizontal avec avancer (qui utilise la collision horizontale), 
* puis le saut, la gravité, et enfin la collision verticale pour arrêter le joueur quand il touche le sol ou le plafond. 
* On ajuste aussi l'état d'animation en fonction de ce que fait le joueur. 
*/

void updatePlayer(Player* player, int* tileMap) {
    // 1. On utilise ta fonction avancer (qui gère l'input et la collision X)
    // Note : On passe tileMap à HorizontaleCollision à l'intérieur d'avancer
    const Uint8* state = SDL_GetKeyboardState(NULL);
    int moveX = 0;
    if (state[SDL_SCANCODE_RIGHT]) moveX = 5;
    if (state[SDL_SCANCODE_LEFT]) moveX = -5;
    
    // Si tu veux rester fidèle à tes fonctions, on les appelle ainsi :
    HorizontaleCollision(player, tileMap, moveX); 
    
    // Mise à jour de l'animation horizontale
    if (moveX > 0) { player->facingLeft = 0; player->animState = ANIM_RUN; }
    else if (moveX < 0) { player->facingLeft = 1; player->animState = ANIM_RUN; }
    else { player->animState = ANIM_IDLE; }

    // 2. On utilise ta fonction saut
    saut(player);

    // 3. On utilise ta fonction descendre (gravité)
    descendre(player);

    // 4. On applique la collision verticale avec la tileMap
    // C'est ce qui arrête le joueur quand il touche les tuiles 6, 7 ou 8
    VerticaleCollision(player, tileMap);

    // Correction de l'état d'animation pour le saut
    if (player->isJumping) {
        player->animState = ANIM_JUMP;
    }

    // Timer d'animation
    player->frameTimer += 0.15f;
}