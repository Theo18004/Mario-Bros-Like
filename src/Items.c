/**
 * @file Items.c
 * @brief Gestion des objets ramassables du jeu (Pièces).
 */
#include "Items.h"
#include <math.h>

void render_coin(SDL_Renderer* renderer, SDL_Texture* texCoin, int x, int y, int camX, int camY) {
    if (!texCoin) return; 

    int displayW = 50; 
    int displayH = 50; 
    
    int floatOffset = (int)(sin(SDL_GetTicks() / 200.0) * 4);
    int offsetX = (32 - displayW) / 2;
    int offsetY = ((32 - displayH) / 2) + floatOffset; 

    SDL_Rect dest = { 
        x - camX + offsetX, 
        y - camY + offsetY, 
        displayW, 
        displayH 
    };
    
    SDL_RenderCopy(renderer, texCoin, NULL, &dest);
}


void render_checkpoint(SDL_Renderer* renderer, SDL_Texture* texCheckpoint, Checkpoint* d, int camX, int camY) {
    if (!texCheckpoint) return;

    int texW, texH;
    SDL_QueryTexture(texCheckpoint, NULL, NULL, &texW, &texH);

    int nbFrames = 26; 
    int frameW = texW / nbFrames; 
    int frameH = texH; 

    int currentFrame = 0;
    
    if (d->actif == 1) {
        // On calcule combien de millisecondes se sont écoulées depuis le contact
        Uint32 timeElapsed = SDL_GetTicks() - d->animStart;
        
        // On calcule on en est à quelle frame (50ms par frame)
        currentFrame = timeElapsed / 50; 
        
        if (currentFrame >= nbFrames) {
            currentFrame = nbFrames - 1; 
        }
    } else {
        currentFrame = 0; 
    }

    SDL_Rect src = { currentFrame * frameW, 0, frameW, frameH };

    SDL_Rect dest = { 
        d->rect.x - camX, 
        d->rect.y - camY, 
        64, 
        64  
    };

    SDL_RenderCopy(renderer, texCheckpoint, &src, &dest);
}