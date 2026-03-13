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
    (void)spawnX;
    (void)spawnY;
    if (p->state != STATE_DEAD) return;
    p->rect.x = p->checkpointX; 
    p->rect.y = p->checkpointY;
    p->velY = 0; p->state = STATE_IDLE; 
    p->lives--; 
    reset_score(s); 
}


void reset_level(Player* p, Ennemi* loup, Thwomp* thwomps, Piece* pieces, Score* s, Camera* cam, int total_reset) {
    // Replacer le joueur au point de départ
    if (total_reset){
        p->lives = 3;
        p->checkpointX = 20; 
        p->checkpointY = 1000;
        
    } 
    init_player(p, p->checkpointX, p->checkpointY);

    // Réinitialiser les ennemis
    init_loupas(loup, 600, 1050); 
    loup->vivant = 1;
    
    init_thwomp(&thwomps[0], 8960, 672);
    init_thwomp(&thwomps[1], 9120, 672);
    init_thwomp(&thwomps[2], 9280, 672);
    init_thwomp(&thwomps[3], 9408, 672);
    init_thwomp(&thwomps[4], 9568, 672);
    init_thwomp(&thwomps[5], 9728, 672);
    init_thwomp(&thwomps[6], 9888, 672);

    // Faire réapparaître les pièces
    for (int i = 0; i < NB_PIECES; i++) {
        pieces[i].vivant = 1;
    }

    // Remettre la caméra et le score
    cam->x = 0; cam->y = 0;
    if (total_reset) {
        init_score(s, s->rect.x, s->rect.y);
    }
}