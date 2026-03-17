/**
 * @file ennemi.c
 * @brief Comportement et rendu de l'ennemi Loup (Sprite Sheet).
 */
#include "ennemi.h"
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
    if (check_collision(e->rect, map, 0)) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    
    SDL_Rect testVide = e->rect;
    testVide.y += 5;
    if (e->direction > 0) testVide.x += e->rect.w;
    else testVide.x -= 5;
    testVide.w = 5;

    if (!check_collision(testVide, map, 0) && e->onGround) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    e->velY += 0.6f;
    e->rect.y += (int)e->velY;
    if (check_collision(e->rect, map, 0)) {
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

void init_thwomp(Thwomp* t, int x, int y) {
    t->rect.x = x; 
    t->rect.y = y;
    t->rect.w = 48; 
    t->rect.h = 64; 
    
    t->startY = y;  
    t->velY = 0.0f;
    t->state = THWOMP_IDLE;
    t->groundTimer = 0;
    t->vivant = 1;
}

void update_thwomp(Thwomp* t, Player* p, int* map) {
    if (!t->vivant) return;
    int tileSize = TILE_SIZE * MAP_SCALE;

    switch (t->state) {
        case THWOMP_IDLE:
            if (p->rect.x + p->rect.w > t->rect.x - 30 && p->rect.x < t->rect.x + t->rect.w + 30) {
                if (p->rect.y > t->rect.y) {
                    t->state = THWOMP_FALLING;
                    t->velY = 0;
                }
            }
            break;

        case THWOMP_FALLING:
            t->velY += 1.5f; 
            t->rect.y += (int)t->velY;

            if (check_collision(t->rect, map,0)) {
                t->rect.y = ((t->rect.y + t->rect.h) / tileSize) * tileSize - t->rect.h;
                t->velY = 0;
                t->state = THWOMP_GROUNDED;
                t->groundTimer = SDL_GetTicks(); 
            }
            break;

        case THWOMP_GROUNDED:
            // Reste au sol pendant 1 seconde (1000 ms)
            if (SDL_GetTicks() - t->groundTimer > 1000) {
                t->state = THWOMP_RISING;
            }
            break;

        case THWOMP_RISING:
            // Remonte doucement
            t->rect.y -= 2; 
            
            // S'il atteint son plafond d'origine
            if (t->rect.y <= t->startY) {
                t->rect.y = t->startY;
                t->state = THWOMP_IDLE;
            }
            break;
    }
}


void render_thwomp(SDL_Renderer* renderer, Thwomp* t, int camX, int camY, SDL_Texture* texThwomp) {
    if (t->vivant) {
        
        int current_frame = 0;
        switch (t->state) {
            case THWOMP_IDLE:     
                current_frame = 0; // Frame 0 : En attente en haut
                break;
            case THWOMP_FALLING:  
                current_frame = 1; // Frame 1 : Descente
                break;
            case THWOMP_GROUNDED: 
                current_frame = 2; // Frame 2 : Écrasé au sol
                break;
            case THWOMP_RISING:   
                current_frame = 3; // Frame 3 : Remontée 
                break;
        }

        int largeur_thwomp_png = 41; 
        int hauteur_thwomp_png = 55; 

        SDL_Rect srcThwomp = { 
            current_frame * largeur_thwomp_png, 
            0,
            largeur_thwomp_png,
            hauteur_thwomp_png
        };

        SDL_Rect destThwomp = { 
            t->rect.x - camX, 
            t->rect.y - camY, 
            t->rect.w, 
            t->rect.h  
        };
        
        SDL_RenderCopy(renderer, texThwomp, &srcThwomp, &destThwomp);
    }
}


void init_podoboo(Podoboo* p, int posX, int limYBas, int limYHaut) {
    p->rect.x = posX;
    p->rect.y = limYBas; // Il commence en bas (caché dans la lave)
    p->rect.w = 32; // Remplace par la largeur de ton monstre
    p->rect.h = 32; // Remplace par la hauteur de ton monstre
    
    p->minY = limYHaut; 
    p->maxY = limYBas;  
    p->speedY = -5.0f;  // Vitesse négative = il monte au départ
    p->vivant = 1;
}

void update_podoboo(Podoboo* p) {
    if (!p->vivant) return;
    
    p->rect.y += (int)p->speedY;
    if (p->rect.y <= p->minY) {
        p->rect.y = p->minY;
        p->speedY = 5.0f; 
    }
    if (p->rect.y >= p->maxY) {
        p->rect.y = p->maxY;
        p->speedY = -5.0f; 
    }
}

void render_podoboo(SDL_Renderer* renderer, Podoboo* p, int camX, int camY, SDL_Texture* texPodoboo) {
    if (!p->vivant || !texPodoboo) return;

    int texW, texH;
    SDL_QueryTexture(texPodoboo, NULL, NULL, &texW, &texH);
    SDL_Rect src = { 0, 0, texW, texH }; 

    SDL_Rect dest = { 
        p->rect.x - camX, 
        p->rect.y - camY, 
        p->rect.w, 
        p->rect.h  
    };

    SDL_RendererFlip flip = (p->speedY > 0) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, texPodoboo, &src, &dest, 0, NULL, flip);
}