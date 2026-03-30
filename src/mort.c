/**
 * @file mort.c
 * @brief Gestion de la machine à état de mort du joueur.
 */
#include "mort.h"
#include "ennemi.h"

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
    (void)s;
    if (p->state != STATE_DEAD) return;
    p->rect.x = p->checkpointX;
    p->rect.y = p->checkpointY;
    p->velY = 0; p->state = STATE_IDLE;
    p->lives--;

    // reinitialise les sons
    Mix_HaltChannel(-1);
}

void reset_level(Player* p, Ennemi* mesLoupas, Thwomp* thwomps, Podoboo* mesPodoboo, Coquilas* mesCoquilas, Ennemi* jc, Ennemi* mesOlaf, Piece* pieces, Score* s, Camera* cam, int total_reset, int levelID) {
    // Replacer le joueur au point de départ
    if (levelID == 1) {
        // --- MAP 1 ---
        if (total_reset){
            p->lives = 3;
            p->checkpointX = 20;
            p->checkpointY = 1000;
        }
        if (total_reset){
            p->lives = 3;
            init_score(s);
        }
        init_player(p, p->checkpointX, p->checkpointY);

        // Sauvegarder l'état des ennemis avant de les réinitialiser
        int etatLoup[NB_LOUPAS], etatJC[NB_JEAN_CLAUDE];
        if (!total_reset) {
            for(int i=0; i<NB_LOUPAS; i++)  etatLoup[i] = mesLoupas[i].vivant;
            for(int i=0; i<NB_JEAN_CLAUDE; i++)  etatJC[i] = jc[i].vivant;
        }

        // Réinitialiser les ennemis
        init_loupas(&mesLoupas[0], 600, 1000);
        init_loupas(&mesLoupas[1], 1184, 768);
        init_loupas(&mesLoupas[2], 1920, 576);
        init_loupas(&mesLoupas[3], 2368, 576);
        init_loupas(&mesLoupas[4], 3072, 1056);

        init_thwomp(&thwomps[0], 8960, 672);
        init_thwomp(&thwomps[1], 9120, 672);
        init_thwomp(&thwomps[2], 9250, 672);
        init_thwomp(&thwomps[3], 9340, 672);
        init_thwomp(&thwomps[4], 9400, 672);
        init_thwomp(&thwomps[5], 9568, 672);
        init_thwomp(&thwomps[6], 9728, 672);
        init_thwomp(&thwomps[7], 9888, 672);

        init_podoboo(&mesPodoboo[0], 7296, 1088, 800);
        init_podoboo(&mesPodoboo[1], 7488, 1088, 800);
        init_podoboo(&mesPodoboo[2], 7648, 1088, 800);
        init_podoboo(&mesPodoboo[3], 7680, 1088, 800);
        init_podoboo(&mesPodoboo[4], 7776, 1088, 800);
        init_podoboo(&mesPodoboo[5], 7840, 1088, 800);
        init_podoboo(&mesPodoboo[6], 7968, 1088, 800);
        init_podoboo(&mesPodoboo[7], 8128, 1088, 800);
        init_podoboo(&mesPodoboo[8], 8192, 1088, 800);
        init_podoboo(&mesPodoboo[9], 8256, 1088, 800);
        init_podoboo(&mesPodoboo[10], 8320, 1088, 800);

        init_coquilas(&mesCoquilas[0], 5896, 650);
        init_coquilas(&mesCoquilas[1], 4416, 608);
        init_coquilas(&mesCoquilas[2], 6272, 960);

        init_jc(&jc[0], 10300, 900);
        init_jc(&jc[1], 10700, 450);
        init_jc(&jc[2], 11500, 750);
        init_jc(&jc[3], 13000, 900);

        //On laisse mort ceux qui étaient morts avant le reset si ce n'est pas un gameover
        if( !total_reset){
            for(int i=0; i<NB_LOUPAS; i++)  mesLoupas[i].vivant = etatLoup[i];
            for(int i=0; i<NB_JEAN_CLAUDE; i++)  jc[i].vivant = etatJC[i];
            
        }
        
    }else if (levelID == 2) {
        // --- MAP 2 ---
        if (total_reset){
            p->lives = 3;
            p->checkpointX = 20;
            p->checkpointY = 1000;
        }
        if (total_reset){
            p->lives = 3;
            init_score(s);
        }
        init_player(p, p->checkpointX, p->checkpointY);

        // Sauvegarder l'état des ennemis avant de les réinitialiser
        int etatLoup[NB_LOUPAS], etatOlaf[NB_OLAF];
        if (!total_reset) {
            for(int i=0; i<NB_LOUPAS; i++)  etatLoup[i] = mesLoupas[i].vivant;
            for(int i=0; i<NB_OLAF; i++)  etatOlaf[i] = mesOlaf[i].vivant;
        }

        // Réinitialiser les ennemis
        init_loupas(&mesLoupas[0], 600, 1000);

        init_snowman(&mesOlaf[0], 1200, 800);
        init_snowman(&mesOlaf[1], 1500, 800);



        // On laisse mort ceux qui étaient morts avant le reset si ce n'est pas un gameover
        if( !total_reset){
            for(int i=0; i<NB_LOUPAS; i++)  mesLoupas[i].vivant = etatLoup[i];
            for(int i=0; i<NB_OLAF; i++)  mesOlaf[i].vivant = etatOlaf[i];
        }

















    }

    // Faire réapparaître les pièces si gameover
    if (total_reset) {
        for (int i = 0; i < NB_PIECES; i++) {
            pieces[i].vivant = 1;
        }
    }
    
    // Remettre la caméra et le score
    cam->x = 0; cam->y = 0;
}
