/**
 * @file ennemi.c
 * @brief Implémentation technique des singes avec SDL2.
 * @internal
 */

#include "ennemi.h"

Monkey InitMonkey(SDL_Texture* texture, SDL_Point pos, float speed) {
    Monkey m;
    m.spriteSheet = texture;
    m.position = pos;
    m.speed = speed;
    m.direction = 1;
    m.currentFrame = 0;
    m.frameTimer = 0.0f;
    m.animFPS = 12.0f;
    
    // Calcul de la taille d'une frame via les requêtes de texture SDL
    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    m.size.x = texW / MONKEY_COLS;
    m.size.y = texH / MONKEY_ROWS;
    
    return m;
}

void UpdateMonkey(Monkey *m, float deltaTime) {
    // Déplacement horizontal simple
    m->position.x += (int)((m->speed * m->direction));

    // Mise à jour du timer d'animation
    m->frameTimer += deltaTime;
    if (m->frameTimer >= (1.0f / m->animFPS)) {
        m->frameTimer = 0;
        m->currentFrame = (m->currentFrame + 1) % MONKEY_TOTAL_FRAMES;
    }
}

void DrawMonkey(SDL_Renderer* renderer, Monkey* m, Camera cam) {
    // Calcul de la zone source (dans le sprite sheet)
    int col = m->currentFrame % MONKEY_COLS;
    int row = m->currentFrame / MONKEY_COLS;
    
    SDL_Rect srcRect = {
        col * m->size.x,
        row * m->size.y,
        m->size.x,
        m->size.y
    };
    
    // Calcul de la zone destination (sur l'écran, avec offset caméra)
    SDL_Rect destRect = {
        m->position.x - cam.x,
        m->position.y - cam.y,
        m->size.x,
        m->size.y
    };
    
    // Inversion du sprite si l'ennemi change de direction
    SDL_RendererFlip flip = (m->direction < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    
    SDL_RenderCopyEx(renderer, m->spriteSheet, &srcRect, &destRect, 0, NULL, flip);
}