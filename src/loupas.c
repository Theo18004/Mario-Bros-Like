/**
 * @file loupas.c
 * @brief Comportement et rendu de l'ennemi Loup (Sprite Sheet).
 */
#include "loupas.h"
#include "collision.h"
#include "defs.h"

void init_loupas(Ennemi* e, int x, int y) {
    e->rect.x = x; 
    e->rect.y = y;
    
    
    e->rect.w = 40; 
    e->rect.h = 28; 
    
    e->velY = 0.0f; 
    e->speed = 2.0f; 
    e->direction = -1; 
    e->onGround = 0;
    e->state = STATE_RUN; 
    e->vivant = 1;
}

void update_loupas(Ennemi* e, int* map) {
    if (!e->vivant) return;
    int oldX = e->rect.x;
    int tileSize = TILE_SIZE * MAP_SCALE;
    
    e->rect.x += (int)(e->speed * e->direction);
    if (check_collision(e->rect, map)) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    
    SDL_Rect testVide = e->rect;
    testVide.y += 5;
    if (e->direction > 0) testVide.x += e->rect.w;
    else testVide.x -= 5;
    testVide.w = 5;

    if (!check_collision(testVide, map) && e->onGround) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    e->velY += 0.6f;
    e->rect.y += (int)e->velY;
    if (check_collision(e->rect, map)) {
        e->rect.y = ((e->rect.y + e->rect.h) / tileSize) * tileSize - e->rect.h;
        e->velY = 0; e->onGround = 1;
    } else e->onGround = 0;
}

void render_loupas(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, 
                   SDL_Texture* texEnnemi) {
    if (!e->vivant || !texEnnemi) return;

    int nbFrames = 5;      
    int frameW = 64;        
    int frameH = 32;        
    int startX = 320;       
    int startY = 128;       

    int currentFrame = (SDL_GetTicks() / 200) % nbFrames;


    SDL_Rect src = { startX + (currentFrame * frameW), startY, frameW, frameH };
    
    int displayW = 56; 
    int displayH = 30; 

    int offsetX = (e->rect.w - displayW) / 2;
    int offsetY = e->rect.h - displayH; 

    SDL_Rect dest = { 
        e->rect.x - scrollX + offsetX, 
        e->rect.y - scrollY + offsetY, 
        displayW, 
        displayH 
    };

    SDL_RendererFlip flip = (e->direction < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    
    SDL_RenderCopyEx(renderer, texEnnemi, &src, &dest, 0, NULL, flip);
}