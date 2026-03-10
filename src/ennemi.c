/**
 * @file ennemi.c
 * @brief Comportement des ennemis (IA de patrouille).
 */
#include "ennemi.h"
#include "collision.h"
#include "defs.h"

void init_ennemi(Ennemi* e, int x, int y) {
    e->rect.x = x; e->rect.y = y;
    e->rect.w = 30; e->rect.h = 48;
    e->velY = 0.0f; e->speed = 2.0f;
    e->direction = 1; e->onGround = 0;
    e->state = STATE_RUN; e->vivant = 1;
}

void update_ennemi(Ennemi* e, int* map) {
    if (!e->vivant) return;
    int oldX = e->rect.x;
    int tileSize = TILE_SIZE * MAP_SCALE;
    
    e->rect.x += (int)(e->speed * e->direction);
    if (check_collision(e->rect, map)) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    // Détection du vide pour faire demi-tour
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

void render_ennemi(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun) {
    if (!e->vivant) return;
    SDL_SetTextureColorMod(texRun, 255, 100, 100); // Teinte rouge
    SDL_SetTextureColorMod(texIdle, 255, 100, 100);

    SDL_Texture* currentTexture = (e->direction != 0) ? texRun : texIdle;
    int nbFrames = (currentTexture == texRun) ? 8 : 6;
    int texW, texH;
    SDL_QueryTexture(currentTexture, NULL, NULL, &texW, &texH);
    int currentFrame = (SDL_GetTicks() / 120) % nbFrames;

    SDL_Rect src = { currentFrame * (texW / nbFrames), 0, (texW / nbFrames), texH };
    SDL_Rect dest = { e->rect.x - scrollX - 17, e->rect.y - scrollY - 16, 64, 64 };
    SDL_RendererFlip flip = (e->direction > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, currentTexture, &src, &dest, 0, NULL, flip);

    SDL_SetTextureColorMod(texRun, 255, 255, 255);
    SDL_SetTextureColorMod(texIdle, 255, 255, 255);
}