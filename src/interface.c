/**
 * @file interface.c
 * @brief Dessin des éléments d'interface utilisateur (HUD).
 */
#include <SDL2/SDL.h>
#include "interface.h"
#include "player.h"
#include "ennemi.h" 
#include "flag.h"   
#include "defs.h"

void render_lives(SDL_Renderer* renderer, SDL_Texture** texVies, int lives) {
    if (lives <= 0) return;

    int index = lives - 1;
    if (index > 4) index = 4;

    if (texVies[index] == NULL) {
        return;
    }

    int imgW, imgH, posX, posY;
    SDL_QueryTexture(texVies[index], NULL, NULL, &imgW, &imgH);

    imgW = 55;
    imgH = 25;
    posX=5;
    posY=10;

    SDL_Rect dest = { posX, posY, imgW, imgH };
    SDL_RenderCopy(renderer, texVies[index], NULL, &dest);
}

void render_pieces_hud(SDL_Renderer* renderer, SDL_Texture** texPiecesHUD, Piece* mesPieces) {
    if (!renderer || !texPiecesHUD || !mesPieces) return;

    int index = 0;
    
    if (mesPieces[0].vivant == 0) index += 1;
    if (mesPieces[1].vivant == 0) index += 2;
    if (mesPieces[2].vivant == 0) index += 4;
    if (mesPieces[3].vivant == 0) index += 8;

    if (texPiecesHUD[index] != NULL) {
        SDL_QueryTexture(texPiecesHUD[index], NULL, NULL, NULL, NULL);
        
        int posX = 5;
        int posY = 40; 
        int imgW = 120;
        int imgH = 40;
        SDL_Rect dest = { posX, posY, imgW, imgH }; 
        SDL_RenderCopy(renderer, texPiecesHUD[index], NULL, &dest);
    }
}

void render_timer(SDL_Renderer* renderer, TTF_Font* font, int tempsRestant) {
    if (!font) return;

    char timeText[32];
    sprintf(timeText, "TIME %03d", tempsRestant);
    SDL_Color color = {255, 255, 255, 255}; 

    SDL_Surface* surface = TTF_RenderText_Solid(font, timeText, color);
    if (!surface) return;
    int posX = 1175;
    int posY = 10;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = { posX-100, posY, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void init_score(Score* s) {
    s->bonus = 0;
    s->value = 0;
    s->max_x = 0;
    s->rect.x = 950;
    s->rect.y = 10;
}

void update_score(Score* s, int player_x) {
    if (player_x > s->max_x) {
        s->max_x = player_x;
    }
    s->value = (s->max_x / 20);
}

void render_score(SDL_Renderer* renderer, TTF_Font* font, Score* s) {
    if (!font || !s) return;

    char scoreText[32];
    sprintf(scoreText, "SCORE %06d", s->value + s->bonus);

    SDL_Color color = {255, 255, 255, 255}; 
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText, color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = { s->rect.x -100, s->rect.y, surface->w, surface->h };

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void reset_score(Score* s) {
    if (s != NULL) {
        s->bonus = 0;
        s->value = 0;
        s->max_x = 0;
    }
}

int gameover(SDL_Renderer* renderer, TTF_Font* font, Player* player, int score_affichage_fin, int meilleur_score) {
    if (player->lives <= 0) {
        int w, h;
        SDL_RenderGetLogicalSize(renderer, &w, &h);
        if (w == 0) SDL_GetRendererOutputSize(renderer, &w, &h);

        int gameOverScreen = 1;
        int action = 0; // 0 = Quitter, 1 = Rejouer

        while (gameOverScreen) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    gameOverScreen = 0;
                    action = 0;
                }
                if (e.type == SDL_KEYDOWN && (e.key.keysym.scancode == SDL_SCANCODE_SPACE || e.key.keysym.scancode == SDL_SCANCODE_RETURN)) {
                    gameOverScreen = 0;
                    action = 1;
                }
            }

            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderClear(renderer);

            if (font != NULL) {
                SDL_Color rouge = {255, 50, 50, 255};
                SDL_Color blanc = {255, 255, 255, 255};
                char buffer[100];

                SDL_Surface* sGO = TTF_RenderText_Solid(font, "GAME OVER", rouge);
                SDL_Texture* tGO = SDL_CreateTextureFromSurface(renderer, sGO);
                SDL_Rect rGO = { w/2 - sGO->w/2, h/2 - 100, sGO->w, sGO->h };
                SDL_RenderCopy(renderer, tGO, NULL, &rGO);
                SDL_FreeSurface(sGO); SDL_DestroyTexture(tGO);

                sprintf(buffer, "Score : %d   |   Meilleur Score : %d", score_affichage_fin, meilleur_score);
                SDL_Surface* sScore = TTF_RenderText_Solid(font, buffer, blanc);
                SDL_Texture* tScore = SDL_CreateTextureFromSurface(renderer, sScore);
                SDL_Rect rScore = { w/2 - sScore->w/2, h/2, sScore->w, sScore->h };
                SDL_RenderCopy(renderer, tScore, NULL, &rScore);
                SDL_FreeSurface(sScore); SDL_DestroyTexture(tScore);

                SDL_Surface* sInst = TTF_RenderText_Solid(font, "Appuyez sur ESPACE pour revenir", blanc);
                SDL_Texture* tInst = SDL_CreateTextureFromSurface(renderer, sInst);
                SDL_Rect rInst = { w/2 - sInst->w/2, h/2 + 100, sInst->w, sInst->h };
                SDL_RenderCopy(renderer, tInst, NULL, &rInst);
                SDL_FreeSurface(sInst); SDL_DestroyTexture(tInst);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        return action;
    }
    return 1;
}

int victory_screen(SDL_Renderer* renderer, TTF_Font* font, Player* player, Score* score, int temps_restant, int etoiles) {
    int w, h;
    SDL_RenderGetLogicalSize(renderer, &w, &h);
    if (w == 0) SDL_GetRendererOutputSize(renderer, &w, &h);

    int victoryScreenActive = 1;
    SDL_Delay(1000);
    int action = 0; 
    int animationFinie = 0; // Passe à 1 quand le temps atteint 0

    while (victoryScreenActive) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                victoryScreenActive = 0;
                action = 0;
            }
            if (e.type == SDL_KEYDOWN && (e.key.keysym.scancode == SDL_SCANCODE_SPACE || e.key.keysym.scancode == SDL_SCANCODE_RETURN)) {
                if (animationFinie) {
                    victoryScreenActive = 0;
                    action = 1;
                } else {
                    score->bonus += temps_restant * 50;
                    temps_restant = 0;
                    animationFinie = 1;
                }
            }
        }

        // --- ANIMATION DU TEMPS ---
        if (!animationFinie) {
            if (temps_restant > 0) {
                
                int vitesse = 5; // Enlève 5s par frame 
                if (temps_restant < vitesse) vitesse = temps_restant;
                
                temps_restant -= vitesse;
                score->bonus += (vitesse * 3);
            } else {
                animationFinie = 1;
            }
        }

        // --- RENDU ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Fond Bleu foncé
        SDL_RenderClear(renderer);

        if (font != NULL) {
            SDL_Color or = {255, 215, 0, 255};
            SDL_Color blanc = {255, 255, 255, 255};
            
            char buffer[100];

            // 1. Titre
            SDL_Surface* sTitre = TTF_RenderText_Solid(font, "NIVEAU TERMINE !", or);
            SDL_Texture* tTitre = SDL_CreateTextureFromSurface(renderer, sTitre);
            SDL_Rect rTitre = { w/2 - sTitre->w/2, h/2 - 200, sTitre->w, sTitre->h };
            SDL_RenderCopy(renderer, tTitre, NULL, &rTitre);
            SDL_FreeSurface(sTitre); SDL_DestroyTexture(tTitre);

            // 2. Score Final 
            sprintf(buffer, "Score Total : %d", score->value + score->bonus);
            SDL_Surface* sScore = TTF_RenderText_Solid(font, buffer, blanc);
            SDL_Texture* tScore = SDL_CreateTextureFromSurface(renderer, sScore);
            SDL_Rect rScore = { w/2 - sScore->w/2, h/2 - 100, sScore->w, sScore->h };
            SDL_RenderCopy(renderer, tScore, NULL, &rScore);
            SDL_FreeSurface(sScore); SDL_DestroyTexture(tScore);

            // 3. Temps restant bonus
            sprintf(buffer, "Bonus Temps : %d", temps_restant);
            SDL_Surface* sTemps = TTF_RenderText_Solid(font, buffer, blanc);
            SDL_Texture* tTemps = SDL_CreateTextureFromSurface(renderer, sTemps);
            SDL_Rect rTemps = { w/2 - sTemps->w/2, h/2 - 40, sTemps->w, sTemps->h };
            SDL_RenderCopy(renderer, tTemps, NULL, &rTemps);
            SDL_FreeSurface(sTemps); SDL_DestroyTexture(tTemps);

            // 4. Stats
            sprintf(buffer, "Vies : %d   |   Etoiles : %d/3", player->lives, etoiles);
            SDL_Surface* sStats = TTF_RenderText_Solid(font, buffer, blanc);
            SDL_Texture* tStats = SDL_CreateTextureFromSurface(renderer, sStats);
            SDL_Rect rStats = { w/2 - sStats->w/2, h/2 + 20, sStats->w, sStats->h };
            SDL_RenderCopy(renderer, tStats, NULL, &rStats);
            SDL_FreeSurface(sStats); SDL_DestroyTexture(tStats);

            // 5. Appuyer sur ESPACE 
            if (animationFinie) {
                SDL_Surface* sInst = TTF_RenderText_Solid(font, "Appuyez sur ESPACE pour continuer", or);
                SDL_Texture* tInst = SDL_CreateTextureFromSurface(renderer, sInst);
                SDL_Rect rInst = { w/2 - sInst->w/2, h/2 + 150, sInst->w, sInst->h };
                SDL_RenderCopy(renderer, tInst, NULL, &rInst);
                SDL_FreeSurface(sInst); SDL_DestroyTexture(tInst);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); 
    }
    return action;
}


void render_debug_hitboxes(SDL_Renderer* renderer, Player* player, 
                           Ennemi* loupas, Thwomp* thwomps, Podoboo* podoboo, 
                           Coquilas* coquilas, Ennemi* jc, Flag* drapeau, 
                           int camX, int camY) {
                           
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // 1. Joueur
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
    SDL_Rect rPlayer = { player->rect.x - camX, player->rect.y - camY, player->rect.w, player->rect.h };
    SDL_RenderFillRect(renderer, &rPlayer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &rPlayer);

    // 2. Loupas
    for (int i = 0; i < NB_LOUPAS; i++) {
        if (loupas[i].vivant) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
            SDL_Rect r = { loupas[i].rect.x - camX, loupas[i].rect.y - camY, loupas[i].rect.w, loupas[i].rect.h };
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &r);
        }
    }

    // 3. Thwomps
    for (int i = 0; i < NB_THWOMPS; i++) {
        if (thwomps[i].vivant) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 128);
            SDL_Rect r = { thwomps[i].rect.x - camX, thwomps[i].rect.y - camY, thwomps[i].rect.w, thwomps[i].rect.h };
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawRect(renderer, &r);
        }
    }

    // 4. Podoboos
    for (int i = 0; i < NB_PODOBOO; i++) {
        if (podoboo[i].vivant) {
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 128);
            SDL_Rect r = { podoboo[i].rect.x - camX, podoboo[i].rect.y - camY, podoboo[i].rect.w, podoboo[i].rect.h };
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
            SDL_RenderDrawRect(renderer, &r);
        }
    }

    // 5. Coquilas
    for (int i = 0; i < NB_COQUILAS; i++) {
        if (coquilas[i].vivant) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 128);
            SDL_Rect r = { coquilas[i].rect.x - camX, coquilas[i].rect.y - camY, coquilas[i].rect.w, coquilas[i].rect.h };
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &r);
        }
    }

    // 6. Jean-Claude
    for (int i = 0; i < NB_JEAN_CLAUDE; i++) {
        if (jc[i].vivant) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
            SDL_Rect r = { jc[i].rect.x - camX, jc[i].rect.y - camY, jc[i].rect.w, jc[i].rect.h };
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &r);
        }
    }

    // 7. Drapeau (Mât)
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 128);
    SDL_Rect rMat = { drapeau->matRect.x - camX, drapeau->matRect.y - camY, drapeau->matRect.w, drapeau->matRect.h };
    SDL_RenderFillRect(renderer, &rMat);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rMat);
}


// FONCTIONS DE MENU PAUSE ---

void init_pause_menu(PauseMenu* pm, SDL_Texture* texIcons, int logicalW, int logicalH) {
    pm->texIcons = texIcons;

    // Découpage automatique sur la grille de bouton.png
    if (texIcons) {
        int texW = 0, texH = 0;
        SDL_QueryTexture(texIcons, NULL, NULL, &texW, &texH);
        int srcBtnW = texW / 12; 
        int srcBtnH = texH / 4;  

        pm->srcPauseIcon = (SDL_Rect){5 * srcBtnW, 2 * srcBtnH, srcBtnW, srcBtnH}; // Engrenage
        pm->srcPlay      = (SDL_Rect){4 * srcBtnW, 2 * srcBtnH, srcBtnW, srcBtnH}; // Jouer
        pm->srcRestart   = (SDL_Rect){2 * srcBtnW, 3 * srcBtnH, srcBtnW, srcBtnH}; // Recommencer
        pm->srcHome      = (SDL_Rect){6 * srcBtnW, 3 * srcBtnH, srcBtnW, srcBtnH}; // Accueil
    } else {
        pm->srcPauseIcon = (SDL_Rect){0, 0, 0, 0};
        pm->srcPlay      = (SDL_Rect){0, 0, 0, 0};
        pm->srcRestart   = (SDL_Rect){0, 0, 0, 0};
        pm->srcHome      = (SDL_Rect){0, 0, 0, 0};
    }

    int btnSize = 100;
    pm->dstPlay    = (SDL_Rect){ logicalW / 2 - 180, logicalH / 2 - 62, btnSize, btnSize }; 
    pm->dstRestart = (SDL_Rect){ logicalW / 2 - 50, logicalH / 2 - 50, btnSize, btnSize };
    pm->dstHome    = (SDL_Rect){ logicalW / 2 + 80, logicalH / 2 - 50, btnSize, btnSize };
    pm->dstPausePos = (SDL_Rect){ logicalW - 80, 5, 50, 50 };
}

void render_pause_menu(SDL_Renderer* renderer, PauseMenu* pm, int enPause, int logicalW, int logicalH) {
    if (!enPause) {
        // En jeu normal : on dessine juste l'engrenage dans le HUD
        if (pm->texIcons) {
            SDL_RenderCopy(renderer, pm->texIcons, &pm->srcPauseIcon, &pm->dstPausePos);
        }
    } else {
        // En mode Pause : Voile noir transparent
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_Rect full = {0, 0, logicalW, logicalH};
        SDL_RenderFillRect(renderer, &full);

        // Boutons du menu
        if (pm->texIcons) {
            SDL_RenderCopy(renderer, pm->texIcons, &pm->srcPlay, &pm->dstPlay);
            SDL_RenderCopy(renderer, pm->texIcons, &pm->srcRestart, &pm->dstRestart);
            SDL_RenderCopy(renderer, pm->texIcons, &pm->srcHome, &pm->dstHome);
        }
    }
}