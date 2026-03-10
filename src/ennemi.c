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

void init_thwomp(Thwomp* t, int x, int y) {
    t->rect.x = x; 
    t->rect.y = y;
    t->rect.w = 48; // Le Thwomp est assez massif
    t->rect.h = 64; 
    
    t->startY = y;  // Il mémorise sa position d'origine
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
            // Si le joueur passe "sous" le Thwomp (intersection sur l'axe X)
            // On ajoute une marge pour qu'il tombe un peu avant que le joueur soit pile dessous
            if (p->rect.x + p->rect.w > t->rect.x - 30 && p->rect.x < t->rect.x + t->rect.w + 30) {
                // Et si le joueur est bien en dessous de lui sur l'axe Y
                if (p->rect.y > t->rect.y) {
                    t->state = THWOMP_FALLING;
                    t->velY = 0;
                }
            }
            break;

        case THWOMP_FALLING:
            // Chute très rapide (gravité forte)
            t->velY += 1.5f; 
            t->rect.y += (int)t->velY;

            // Collision avec le sol
            if (check_collision(t->rect, map)) {
                t->rect.y = ((t->rect.y + t->rect.h) / tileSize) * tileSize - t->rect.h;
                t->velY = 0;
                t->state = THWOMP_GROUNDED;
                t->groundTimer = SDL_GetTicks(); // Déclenche le chrono
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

void render_thwomp(SDL_Renderer* renderer, Thwomp* t, int scrollX, int scrollY, SDL_Texture* texThwomp) {
    if (!t->vivant || !texThwomp) return;

    // --- PARAMÈTRES DE LA SPRITE SHEET (thwomp.png) ---
    // On cible la première ligne (les bleus classiques)
    int frameW = 36; // Largeur estimée d'une frame
    int frameH = 52; // Hauteur estimée d'une frame
    
    int currentFrame = 0; // Par défaut : IDLE
    
    // Changement d'expression faciale selon l'état
    if (t->state == THWOMP_FALLING) {
        currentFrame = 2; // Visage énervé
    } else if (t->state == THWOMP_GROUNDED) {
        currentFrame = 3; // Visage impact/grimaçant
    } else if (t->state == THWOMP_RISING) {
        currentFrame = 1; // Visage tendu
    }

    SDL_Rect src = { currentFrame * frameW, 0, frameW, frameH };
    
    // Affichage légèrement plus grand que la hitbox pour le style visuel
    SDL_Rect dest = { 
        t->rect.x - scrollX - 5, 
        t->rect.y - scrollY - 5, 
        t->rect.w + 10, 
        t->rect.h + 10 
    };

    SDL_RenderCopy(renderer, texThwomp, &src, &dest);
}