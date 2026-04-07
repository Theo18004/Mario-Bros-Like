/**
 * @file player.c
 * @brief Physique, contrôles et animation du joueur.
 */
#include "player.h"
#include "collision.h"
#include "defs.h"
#include "mort.h"
#include "interface.h"
#include <math.h>

extern Mix_Chunk *sonSaut;

void init_player(Player* p, int x, int y) {
    p->rect.x = x;
    p->rect.y = y;
    p->rect.w = 20; 
    p->rect.h = 42; 
    p->velY = 0.0f;
    p->velX = 0.0f; 
    p->facingRight = 1;
    p->onGround = 0;
    p->state = STATE_IDLE;
    p->checkpointX = x;
    p->checkpointY = y;
}

void update_player(Player* p, const Uint8* keys, int* map, int levelID) {
    int mapPixelWidth = TILE_SIZE * MAP_SCALE * MAP_WIDTH;

    if (p->state == STATE_DEAD) {
        p->velY += 0.6f;
        p->rect.y += (int)p->velY;
        return;
    }
    if( p->state == STATE_WIN) return;

    int wasOnGround = p->onGround;
    int oldX = p->rect.x;
    int oldY = p->rect.y;

    // --- 1. Paramètres de physique (Valeurs par défaut) ---
    float accel = 0.7f;      
    float friction = 0.0f;  // Arrêt instant hors lune 
    float maxSpeed = 5.0f;   
    float slowDown = 0.6f;  
    float gravity = 0.6f;
    float jumpBoost = -12.0f;

    // --- 2. Adaptation selon le BIOME (Lune) ---
    int currentTileX = p->rect.x / (TILE_SIZE * MAP_SCALE); 
    if (levelID == 2 && currentTileX >= 150 && currentTileX < 300) {
        gravity = 0.3f;  // gravité faible ( apesanteur )
    }

    // --- 3. Adaptation selon le SOL (Glace) ---
    if (check_on_ice(p->rect, map, levelID)) {
        accel = 0.12f;      
        friction = 1.2f;  
        maxSpeed = 10.5f;    
        slowDown = 0.05f;   
    }

    // --- 4. Mouvement Horizontal ---
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        if (p->velX > 0) p->velX -= slowDown;
        p->velX -= accel;
        p->facingRight = 0;
    } 
    else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        if (p->velX < 0) p->velX += slowDown;
        p->velX += accel;
        p->facingRight = 1;
    } 
    else {
        p->velX *= friction; 
        if (fabsf(p->velX) < 0.1f) p->velX = 0;
    }

    if (p->velX > maxSpeed) p->velX = maxSpeed;
    if (p->velX < -maxSpeed) p->velX = -maxSpeed;

    p->rect.x += (int)p->velX;

    // Collisions X
    if (check_collision(p->rect, map, 0, levelID)) {
        int success = 0;
        if (wasOnGround) {
            for (int i = 1; i <= 10; i++) {
                p->rect.y = oldY - i;
                if (!check_collision(p->rect, map, 0, levelID)) { success = 1; break; }
            }
        }
        if (!success) {
            p->rect.y = oldY;
            if (p->rect.x > oldX) while (check_collision(p->rect, map, 0, levelID)) p->rect.x -= 1;
            else if (p->rect.x < oldX) while (check_collision(p->rect, map, 0, levelID)) p->rect.x += 1;
            p->velX = 0; 
        }
    }

    // --- 5. Saut et Gravité ---
    if ((keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) && p->onGround) {
        p->velY = jumpBoost; // Utilise la force calculée (boostée ou normale)
        p->onGround = 0;
        if (sonSaut) Mix_PlayChannel(0, sonSaut, 0);
    }

    p->velY += gravity; // Utilise la gravité calculée (faible ou normale)
    if (p->velY > 15.0f) p->velY = 15.0f;
    p->rect.y += (int)p->velY;

    // Collisions Y
    p->onGround = 0;
    int check_demi = (p->velY > 0) ? 1 : 0;
    if (check_collision(p->rect, map, check_demi, levelID)) {
        if (p->velY > 0) {
            while (check_collision(p->rect, map, check_demi, levelID)) p->rect.y -= 1;
            p->onGround = 1;
        } else {
            while (check_collision(p->rect, map, 0, levelID)) p->rect.y += 1;
        }
        p->velY = 0;
    } else {
        p->rect.y += 1;
        if (check_collision(p->rect, map, 1, levelID)) p->onGround = 1;
        p->rect.y -= 1;
    }

    // --- 6. État d'animation ---
    if (!p->onGround) p->state = STATE_JUMP;
    else if (fabsf(p->velX) > 0.5f) p->state = STATE_RUN;
    else p->state = STATE_IDLE;

    if (p->rect.x < 0) p->rect.x = 0;
    if (p->rect.x + p->rect.w > mapPixelWidth) p->rect.x = mapPixelWidth - p->rect.w;
}

void render_player(SDL_Renderer* renderer, Player* p, int scrollX, int scrollY,
                   SDL_Texture* texIdle, SDL_Texture* texRun, SDL_Texture* texJump, SDL_Texture* texDead) {
    SDL_Texture* currentTexture = texIdle;
    int nbFrames = 6;
    if (p->state == STATE_RUN) { currentTexture = texRun; nbFrames = 8; }
    else if (p->state == STATE_JUMP) { currentTexture = texJump; nbFrames = 10; }
    else if (p->state == STATE_DEAD) { currentTexture = texDead; nbFrames = 3; }

    if (currentTexture) {
        int texW, texH;
        SDL_QueryTexture(currentTexture, NULL, NULL, &texW, &texH);
        int frameW = texW / nbFrames;
        int frame = (SDL_GetTicks() / 100) % nbFrames;
        SDL_Rect src = { frame * frameW, 0, frameW, texH };
        SDL_Rect dest = { p->rect.x - scrollX - (64 - p->rect.w)/2, p->rect.y - scrollY - (64 - p->rect.h), 64, 64 };
        SDL_RendererFlip flip = (p->facingRight) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(renderer, currentTexture, &src, &dest, 0, NULL, flip);
    }
}