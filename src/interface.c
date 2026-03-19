/**
 * @file interface.c
 * @brief Dessin des éléments d'interface utilisateur (HUD).
 */
#include <SDL2/SDL.h>
#include "interface.h"

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
    posY=40;

    SDL_Rect dest = { posX, posY, imgW, imgH };
    SDL_RenderCopy(renderer, texVies[index], NULL, &dest);
}

void render_progress_bar(SDL_Renderer* renderer, int playerX, int mapPixelWidth) {
    int w, h;
    SDL_RenderGetLogicalSize(renderer, &w, &h);
    if (w == 0) SDL_GetRendererOutputSize(renderer, &w, &h);
    int barW = 500; int barH = 10;
    int barX = (w - barW) / 2; int barY = 15;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond
    SDL_Rect bgBar = { barX, barY, barW, barH };
    SDL_RenderFillRect(renderer, &bgBar);

    float progress = (float)playerX / (float)mapPixelWidth;
    int cursorX = barX + (int)(progress * barW);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Curseur
    SDL_Rect cursor = { cursorX - 5, barY - 5, 10, 20 };
    SDL_RenderFillRect(renderer, &cursor);
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
