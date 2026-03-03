#include "player.h"
#include "collision.h"
#include "defs.h"

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

void update_player(Player* p, const Uint8* keys, int* map) {
    p->state = STATE_IDLE; // Reset état par défaut

    // --- Mouvement X ---
    float speed = 5.0f;
    if (keys[SDL_SCANCODE_LEFT]) {
        p->rect.x -= (int)speed;
        p->facingRight = 0;
        p->state = STATE_RUN;
        if (check_collision(p->rect, map)) p->rect.x += (int)speed;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        p->rect.x += (int)speed;
        p->facingRight = 1;
        p->state = STATE_RUN;
        if (check_collision(p->rect, map)) p->rect.x -= (int)speed;
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

    // --- Capteur de Sol (Correction Jump Infini) ---
    SDL_Rect groundCheck = p->rect;
    groundCheck.y += 1;
    if (check_collision(groundCheck, map) && p->velY >= 0) {
        p->onGround = 1;
    } else {
        p->onGround = 0;
    }

    // --- Correction État ---
    if (!p->onGround) {
        p->state = STATE_JUMP;
    }
}

void render_player(SDL_Renderer* renderer, Player* p, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun, SDL_Texture* texJump) {
    
    SDL_Texture* currentTexture = texIdle;
    int nbFrames = 6;

    if (p->state == STATE_RUN) {
        currentTexture = texRun;
        nbFrames = 8;
    } else if (p->state == STATE_JUMP) {
        currentTexture = texJump;
        nbFrames = 10;
    }

    if (currentTexture) {
        int texW, texH;
        SDL_QueryTexture(currentTexture, NULL, NULL, &texW, &texH);
        int singleFrameW = texW / nbFrames;
        
        int currentFrame = 0;
        if (p->state == STATE_RUN) {
            currentFrame = (SDL_GetTicks() / 100) % nbFrames;
        } else if (p->state == STATE_JUMP) {
            currentFrame = (SDL_GetTicks() / 100) % nbFrames;
        } else {
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