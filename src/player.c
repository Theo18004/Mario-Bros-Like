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
    p->rect.w = 30; // Largeur Hitbox
    p->rect.h = 48; // Hauteur Hitbox
    p->velY = 0.0f;
    p->facingRight = 1;
    p->onGround = 0;
    p->state = STATE_IDLE;
}

void update_player(Player* p, const Uint8* keys, int* map, Score* s){

    int mapPixelHeight = TILE_SIZE * MAP_SCALE * MAP_HEIGHT;
    int mapPixelWidth = TILE_SIZE * MAP_SCALE * MAP_WIDTH;


    // On verifie si le joueurs est mort avant de faire les calculs de mouvement*
    if (verifier_conditions_mort(p, mapPixelHeight)) {
        gerer_mort_joueur(p, 20, 1000, s);
        return; 
    }

    if (p->state == STATE_DEAD) {
        int limiteSolChute = mapPixelHeight - 65; // limite chute pour arret animation de mort avant respawn (mettre le même dans mort.c)

        //On laisse tomber le joueur jusqu'à la limite choisie pour qu'on puisse voir l'animation de mort avant de le faire respawn
        if (p->rect.y < limiteSolChute) {
            p->velY += 0.2f; 
            p->rect.y += (int)p->velY;
        } 
        else {
            // on bloque le joueur averc l'animation pour qu'on puisse voir l'animation de mort avant de respawn
            p->rect.y = limiteSolChute;
            p->velY = 0; 
        }
        return; 
    }

    // Si le joueur est en vie, on traite les mouvements normalement
    p->state = STATE_IDLE; // Reset état par défaut

   // --- Mouvement X avec Step-Up pour Diagonales ---
    float speed = 5.0f;
    int oldX = p->rect.x;
    int oldY = p->rect.y;
    int max_step = 10; 
    
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) {
        if (keys[SDL_SCANCODE_LEFT]) {
            p->rect.x -= (int)speed;
            p->facingRight = 0;
        } else {
            p->rect.x += (int)speed;
            p->facingRight = 1;
        }
        p->state = STATE_RUN;

        // Si on touche la pente
        if (check_collision(p->rect, map)) {
            int success = 0;
            // On monte légèrement pour suivre la pente
            for (int i = 1; i <= max_step; i++) {
                p->rect.y = oldY - i;
                if (!check_collision(p->rect, map)) {
                    success = 1;
                    p->velY = 0; 
                    break;
                }
            }

            // Si c'est un vrai mur on bloque le joueur
            if (!success) {
                p->rect.x = oldX;
                p->rect.y = oldY;
                p->state = STATE_IDLE;
            }
        }
    } else {
        if (p->onGround) p->state = STATE_IDLE;
    }

    //Bloqué le joueur au bordure de la map
    //Bloqué à gauche de la map
    if ( p->rect.x < 0) {
        p->rect.x = 0;
    }

    //Bloqué à droite de la map 
    if ( p->rect.x + p->rect.w > mapPixelWidth) {
        p->rect.x = mapPixelWidth - p->rect.w;
    }

    // --- Saut ---
    if (keys[SDL_SCANCODE_UP] && p->onGround) {
        p->velY = -12.0f;
        p->onGround = 0;
    }

    // --- Physique Y ---
    p->velY += 0.6f;
    p->rect.y += (int)p->velY;
    p->onGround = 0;

    if (check_collision(p->rect, map)) {
        if (p->velY > 0) { // Atterrissage
            int tileSize = TILE_SIZE * MAP_SCALE;
            p->rect.y = ((p->rect.y + p->rect.h) / tileSize) * tileSize - p->rect.h;
            if (check_collision(p->rect, map)) p->rect.y -= 1;
            p->velY = 0;
        }
        else if (p->velY < 0) { // Plafond
            int tileSize = TILE_SIZE * MAP_SCALE;
            p->rect.y = (p->rect.y / tileSize + 1) * tileSize;
            p->velY = 0;
        }
    }

    // --- Capteur de Sol (Sticky Ground pour pentes) ---
    SDL_Rect groundCheck = p->rect;
    
    // On regarde 4 pixels plus bas au lieu de 1 pour "coller" à la pente
    groundCheck.y += 4; 
    
    if (check_collision(groundCheck, map) && p->velY >= 0) {
        p->onGround = 1;
        // Si on est sur une pente et qu'on ne saute pas, on force la vitesse Y à 0
        if (p->velY > 0) p->velY = 0; 
    } else {
        p->onGround = 0;
    }

    // --- Correction État Animation ---
    if (!p->onGround) {
        p->state = STATE_JUMP;
    } else if (p->state == STATE_JUMP) {
        // Si on vient d'atterrir, on repasse en IDLE ou RUN
        p->state = (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) ? STATE_RUN : STATE_IDLE;
    }

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
    
    // On met les animations à jour en fonction du temps pour que ça soit fluide et pas lié au framerate
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
            if (deathStartTime == 0) deathStartTime = SDL_GetTicks(); // lance un timer pour l'animation de mort
            currentFrame = (SDL_GetTicks() - deathStartTime) / 150;
            if (currentFrame >= nbFrames) currentFrame = nbFrames - 1; // utilise le timer pour bloquer sur l'animation sur ventre à la fin
        } else {
            deathStartTime = 0;
            currentFrame = (SDL_GetTicks() / 150) % nbFrames;
        }

        SDL_Rect srcP = { currentFrame * singleFrameW, 0, singleFrameW, texH };
        int displaySize = 64;
        // Centrage Hitbox 
        SDL_Rect destP = { p->rect.x - scrollX - 17, p->rect.y - scrollY - 16, displaySize, displaySize };
        
        SDL_RendererFlip flip = (p->facingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(renderer, currentTexture, &srcP, &destP, 0, NULL, flip);
    }
}