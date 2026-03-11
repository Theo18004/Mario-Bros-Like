/**
 * @file player.c
 * @brief Physique, contrôles et animation du joueur.
 */
#include "player.h"
#include "collision.h"
#include "defs.h"
#include "mort.h"
#include "score.h"

void init_player(Player* p, int x, int y) {
    p->rect.x = x;
    p->rect.y = y;
    p->rect.w = 20; // Largeur Hitbox
    p->rect.h = 48; // Hauteur Hitbox
    p->velY = 0.0f;
    p->facingRight = 1;
    p->onGround = 0;
    p->state = STATE_IDLE;
}

void update_player(Player* p, const Uint8* keys, int* map) {
    int mapPixelWidth = TILE_SIZE * MAP_SCALE * MAP_WIDTH;

    if (p->state == STATE_DEAD){
        p->velY += 0.6f;
        p->rect.y += (int)p->velY;
        return;
    }

    // --- 2. Initialisation de la frame ---
    int wasOnGround = p->onGround;
    p->state = STATE_IDLE; 
    int oldX = p->rect.x;
    int oldY = p->rect.y;

    // --- 3. Saut ---
    if (keys[SDL_SCANCODE_UP] && p->onGround) {
        p->velY = -12.0f; // Force du saut
        p->onGround = 0;
    }

    // --- 4. Mouvements sur l'axe X ---
    float speed = 5.0f;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) {
        if (keys[SDL_SCANCODE_LEFT]) {
            p->rect.x -= (int)speed;
            p->facingRight = 0;
        } else {
            p->rect.x += (int)speed;
            p->facingRight = 1;
        }
        
        p->state = STATE_RUN;

        // Si on rentre dans un mur ou une pente
        if (check_collision(p->rect, map)) {
            int success = 0;
            int max_step = 10;
            
            // On tente de monter la pente/escalier  
            if (wasOnGround) {
                for (int i = 1; i <= max_step; i++) {
                    p->rect.y = oldY - i;
                    if (!check_collision(p->rect, map)) {
                        success = 1;
                        break;
                    }
                }
            }

            if (!success) {
                p->rect.y = oldY; 
                if (keys[SDL_SCANCODE_RIGHT]) {
                    while (check_collision(p->rect, map)) p->rect.x -= 1;
                } else if (keys[SDL_SCANCODE_LEFT]) {
                    while (check_collision(p->rect, map)) p->rect.x += 1;
                }
            }
        }
    }

    // Bloquer les bords de la map en X
    if (p->rect.x < 0) p->rect.x = 0;
    if (p->rect.x + p->rect.w > mapPixelWidth) {
        p->rect.x = mapPixelWidth - p->rect.w;
    }

    // --- 5. Gravité et axe Y ---
    p->velY += 0.6f; 
    if (p->velY > 15.0f) p->velY = 15.0f; // Vitesse de chute max

    p->rect.y += (int)p->velY;
    p->onGround = 0; 

    // Résolution des collisions Y par "Pushback"
    if (check_collision(p->rect, map)) {
        if (p->velY > 0) { 
            while (check_collision(p->rect, map)) {
                p->rect.y -= 1; 
            }
            p->velY = 0;
            p->onGround = 1; 
        } 
        else if (p->velY < 0) { 
            while (check_collision(p->rect, map)) {
                p->rect.y += 1; 
            }
            p->velY = 0;
        }
    } else {
        p->rect.y += 1;
        if (check_collision(p->rect, map)) {
            p->onGround = 1; 
        }
        p->rect.y -= 1; 
    }

    // --- 6. Choix final de l'animation ---
    if (!p->onGround) {
        p->state = STATE_JUMP;
    } else if (p->rect.x == oldX && (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT])) { 
        p->state = STATE_IDLE;
    }
    return;
}

void render_player(SDL_Renderer* renderer, Player* p, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun, SDL_Texture* texJump, SDL_Texture* texDead) {
    
    SDL_Texture* currentTexture = texIdle;
    int nbFrames = 6;

    if (p->state == STATE_RUN) {
        currentTexture = texRun;
        nbFrames = 8;
    } else if (p->state == STATE_JUMP) {
        currentTexture = texJump;
        nbFrames = 10;
    } else if (p->state == STATE_DEAD) {
        currentTexture = texDead;
        nbFrames = 3;
    }
    
    // On met les animations à jour en fonction du temps
    if (currentTexture) {
        int texW, texH;
        SDL_QueryTexture(currentTexture, NULL, NULL, &texW, &texH);
        int singleFrameW = texW / nbFrames;

        static Uint32 deathStartTime = 0;
        int currentFrame = 0;

        if (p->state == STATE_RUN) {
            currentFrame = (SDL_GetTicks() / 100) % nbFrames;
        } else if (p->state == STATE_JUMP) {
            currentFrame = (SDL_GetTicks() / 100) % nbFrames;
        } else if (p->state == STATE_DEAD) {
            if (deathStartTime == 0) deathStartTime = SDL_GetTicks(); 
            currentFrame = (SDL_GetTicks() - deathStartTime) / 150;
            if (currentFrame >= nbFrames) currentFrame = nbFrames - 1; 
        } else {
            deathStartTime = 0;
            currentFrame = (SDL_GetTicks() / 150) % nbFrames;
        }

        SDL_Rect srcP = { currentFrame * singleFrameW, 0, singleFrameW, texH };
        
        // Taille d'affichage à l'écran
        int displaySize = 64;

        
        // 1. Centrage horizontal : (Taille de l'image - Taille de la Hitbox) / 2
        int offsetX = (displaySize - p->rect.w) / 2; 
        
        // 2. Alignement vertical : On aligne les pieds de l'image avec le bas de la hitbox
        int offsetY = displaySize - p->rect.h;       

        
        SDL_Rect destP = { 
            p->rect.x - scrollX - offsetX, 
            p->rect.y - scrollY - offsetY, 
            displaySize, 
            displaySize 
        };
        
        SDL_RendererFlip flip = (p->facingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(renderer, currentTexture, &srcP, &destP, 0, NULL, flip);
    }
}