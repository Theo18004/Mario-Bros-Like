/**
 * @file mort.c
 * @brief Gestion de la machine à état de mort du joueur.
 */
#include "mort.h"

int verifier_conditions_mort(Player* p, int mapPixelHeight){
    static Uint32 momentImpactSol = 0;
    if (p->state == STATE_DEAD){
       int limiteSolChute = mapPixelHeight - 65;
       if (p->rect.y >= limiteSolChute) {
            if (momentImpactSol == 0) momentImpactSol = SDL_GetTicks();
            if (SDL_GetTicks() - momentImpactSol > 1000) { momentImpactSol = 0; return 1; }
        }
        return 0; 
    }
    if( p->rect.y > mapPixelHeight - 130){ p->state = STATE_DEAD; p->velY = -3.0f; return 0; }
    return 0; 
}

void gerer_mort_joueur(Player* p, int spawnX, int spawnY, Score* s){
    if (p->state != STATE_DEAD) return;
    p->rect.x = spawnX; p->rect.y = spawnY;
    p->velY = 0; p->state = STATE_IDLE; 
    p->lives--; reset_score(s); 
}