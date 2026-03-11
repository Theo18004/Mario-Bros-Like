#include "Items.h"

void render_coin(SDL_Renderer* renderer, SDL_Texture* texCoin, int x, int y, int camX, int camY) {
    // 1. DÉCOUPE PRÉCISE (Source)
    // On prend la case en bas à droite de ton image 32x32
    SDL_Rect srcCoin = { 16, 16, 16, 16 }; 

    // 2. POSITION ÉCRAN (Destination)
    SDL_Rect destCoin = { 
        x - camX, 
        y - camY, 
        32, // On l'affiche en 32x32 (zoom x2)
        32
    };
    
    SDL_RenderCopy(renderer, texCoin, &srcCoin, &destCoin);
}