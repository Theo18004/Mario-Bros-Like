/**
 * @file interface.c
 * @brief Dessin des éléments d'interface utilisateur (HUD).
 */
#include <SDL2/SDL.h>
#include "interface.h"

void render_lives(SDL_Renderer* renderer, SDL_Texture* texHeart, int lives) {
    int maxLives = 3; 
    int w, h;
    SDL_RenderGetLogicalSize(renderer, &w, &h);
    if (w == 0) SDL_GetRendererOutputSize(renderer, &w, &h);
    int CoX = w - (maxLives * 30) + 20; 

    for (int i = 0; i < lives; i++) {
        int PosX = CoX + ((maxLives - 1 - i) * 20); 
        SDL_Rect dest = { PosX, 10, 32, 32 };  
        SDL_RenderCopy(renderer, texHeart, NULL, &dest);
    }
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