#include "mort.h"

// Vérifie toutes les conditions qui peuvent tuer le joueur ( hors map / enemis)
int verifier_conditions_mort(Player* p, int mapPixelHeight){
    static Uint32 momentImpactSol = 0; // Timer pour le temps quand il touche la limite de chute

    //On vérifie que le joueur est pas déjà mort
    if (p->state == STATE_DEAD){
        int limiteSolChute = mapPixelHeight - 65; // limite chute pour arret animation de mort avant respawn (mettre le même dans player.c)

       if (p->rect.y >= limiteSolChute) {
            //On lance le timer au moment de l'impact avec le sol
            if (momentImpactSol == 0) momentImpactSol = SDL_GetTicks();

            // Si 2000ms (2s) se sont écoulées depuis l'impact on respawn le joueur
            if (SDL_GetTicks() - momentImpactSol > 2000) {
                momentImpactSol = 0;
                return 1; 
            }
        }
        return 0; 
    }

    // Condition de mort du joueur tombe sous la limite
    if( p->rect.y > mapPixelHeight - 130 && p->state != STATE_DEAD){
        p->state = STATE_DEAD;
        p->velY = -3.0f;
        momentImpactSol = 0; 
        return 0; 
    }

    //Si il touche un ennemi ( à faire )

    //Si il touche un trap ( à faire )

    return 0; // Pas de condition de mort vérifiée
}


// Gère l'action de mourir ( animation / respawn)
void gerer_mort_joueur(Player* p, int spawnX, int spawnY){

    //On vérifie que le joueur est bien mort
     if (p->state != STATE_DEAD){
        return;
     }

     // Ajouter une animation de mort ( à faire )


     // Respawn
    if (p->state == STATE_DEAD){
        p->rect.x = spawnX;
        p->rect.y = spawnY;
        p->velY = 0;
        p->state = STATE_IDLE; 
    }
}
