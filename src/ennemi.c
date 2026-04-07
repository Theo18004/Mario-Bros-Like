/**
 * @file ennemi.c
 * @brief Comportement et rendu de l'ennemi Loup (Sprite Sheet).
 */
#include "ennemi.h"
#include "collision.h"
#include "camera.h"
#include "defs.h"

//==============================================================
//========================Loupas================================
//==============================================================

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

void update_loupas(Ennemi* e, int* map, int levelID) {
    if (!e->vivant) return;
    int oldX = e->rect.x;
    int tileSize = TILE_SIZE * MAP_SCALE;
    
    e->rect.x += (int)(e->speed * e->direction);
    if (check_collision(e->rect, map, 0, levelID)) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    
    SDL_Rect testVide = e->rect;
    testVide.y += 5;
    if (e->direction > 0) testVide.x += e->rect.w;
    else testVide.x -= 5;
    testVide.w = 5;

    if (!check_collision(testVide, map, 0, levelID) && e->onGround) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    e->velY += 0.6f;
    e->rect.y += (int)e->velY;
    if (check_collision(e->rect, map, 0, levelID)) {
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


//==============================================================
//========================Thwomp================================
//==============================================================


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

void update_thwomp(Thwomp* t, Player* p, int* map, int levelID) {
    if (!t->vivant) return;
    int tileSize = TILE_SIZE * MAP_SCALE;

    switch (t->state) {
        case THWOMP_IDLE:
            if (p->rect.x + p->rect.w > t->rect.x - 30 && p->rect.x < t->rect.x + t->rect.w + 30) {
                if (p->rect.y > t->rect.y) {
                    t->state = THWOMP_FALLING;
                    if(sonThwomp != NULL) {
                        Mix_PlayChannel(1, sonThwomp, 0);
                    }
                    t->velY = 0;
                }
            }
            break;

        case THWOMP_FALLING:
            t->velY += 1.5f; 
            t->rect.y += (int)t->velY;

            if (check_collision(t->rect, map, 0, levelID)) {
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


//==============================================================
//========================Podoboo===============================
//==============================================================


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

void update_podoboo(Podoboo* p, Camera * cam) {
    if (!p->vivant) return;
    
    p->rect.y += (int)p->speedY;
    if (p->rect.y <= p->minY) {
        p->rect.y = p->minY;
        p->speedY = 5.0f; 

        if(p->rect.x >= cam->x - 50 && p->rect.x <= cam->x + cam->w + 50) {
            if (bouleFeu != NULL) {
                Mix_PlayChannel(2, bouleFeu, 0);
            }
        }
    }
    if (p->rect.y >= p->maxY) {
        p->rect.y = p->maxY;
        p->speedY = -5.0f; 

        if(p->rect.x >= cam->x - 50 && p->rect.x <= cam->x + cam->w + 50) {
            if (bouleFeu != NULL) {
                Mix_PlayChannel(2, bouleFeu, 0);
            }
        }
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


//==============================================================
//========================Coquilas==============================
//==============================================================

void init_coquilas(Coquilas* c, int x, int y) {
    c->rect.x = x; 
    c->rect.y = y;
    c->rect.w = 40; 
    c->rect.h = 28; 

    c->velY = 0.0f; 
    c->speed = 2.0f; 
    c->direction = -1; 
    c->onGround = 0;
    c->state = STATE_RUN; 
    c->vivant = 1;
}

void update_coquilas(Coquilas* c, int* map, int levelID) {
    if (!c->vivant) return;
    int oldX = c->rect.x;
    int tileSize = TILE_SIZE * MAP_SCALE;

    c->rect.x += (int)(c->speed * c->direction);
    if (check_collision(c->rect, map, 0, levelID)) {
        c->rect.x = oldX;
        c->direction *= -1;
    }

    SDL_Rect testVide = c->rect;
    testVide.y += 5;
    if (c->direction > 0) testVide.x += c->rect.w;
    else testVide.x -= 5;
    testVide.w = 5;

    if (!check_collision(testVide, map, 0, levelID) && c->onGround) {
        c->rect.x = oldX;
        c->direction *= -1;
    }

    c->velY += 0.6f;
    c->rect.y += (int)c->velY;
    if (check_collision(c->rect, map, 0, levelID)) {
        c->rect.y = ((c->rect.y + c->rect.h) / tileSize) * tileSize - c->rect.h;
        c->velY = 0; c->onGround = 1;
    } else c->onGround = 0;
}


void render_coquilas(SDL_Renderer* renderer, Coquilas* c, int scrollX, int scrollY, SDL_Texture* texCoquilas) {
    if (!c->vivant || !texCoquilas) return;

    int frameW = 350; 
    int frameH = 256; 
    int nbFrames = 4;
    
    int currentFrame = (SDL_GetTicks() / 150) % nbFrames;

    SDL_Rect src;
    src.x = currentFrame * frameW; 
    src.y = 0;
    src.w = frameW;
    src.h = frameH;

    int displayW = 40; 
    int displayH = 30; 
    int offsetX = (c->rect.w - displayW) / 2; 
    int offsetY = c->rect.h - displayH; 

    SDL_Rect dest = { 
        c->rect.x - scrollX + offsetX, 
        c->rect.y - scrollY + offsetY, 
        displayW, 
        displayH 
    };

    SDL_RendererFlip flip = (c->direction > 0) ? SDL_FLIP_NONE: SDL_FLIP_HORIZONTAL ;
    SDL_RenderCopyEx(renderer, texCoquilas, &src, &dest, 0, NULL, flip);
}


//==============================================================
//========================Jean-Claude===========================
//==============================================================


void init_jc(Ennemi* e, int x, int y) {
    e->rect.x = x; 
    e->rect.y = y;
    e->rect.w = 24; 
    e->rect.h = 44; 
    e->velY = 0.0f; 
    e->speed = 1.0f; 
    e->direction = -1; 
    e->onGround = 0;
    e->state = STATE_RUN; 
    e->vivant = 1;
}



void update_jc(Ennemi* e, int* map, int levelID) {

    if (!e->vivant) return;

    int oldX = e->rect.x;
    int tileSize = TILE_SIZE * MAP_SCALE;
    e->rect.x += (int)(e->speed * e->direction);

    // Collision Mur
    if (check_collision(e->rect, map, 0, levelID)) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    SDL_Rect testVide = e->rect;
    testVide.y += 5;
    if (e->direction > 0) testVide.x += e->rect.w;
    else testVide.x -= 5;
    testVide.w = 5;

    if (!check_collision(testVide, map, 0, levelID) && e->onGround) {
        e->rect.x = oldX;
        e->direction *= -1;
    }
    e->velY += 0.6f;
    e->rect.y += (int)e->velY;

    if (check_collision(e->rect, map, 0, levelID)) {
        e->rect.y = ((e->rect.y + e->rect.h) / tileSize) * tileSize - e->rect.h;
        e->velY = 0; 
        e->onGround = 1;
    } else {
        e->onGround = 0;
    }
}



void render_jc(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, SDL_Texture* texJeanClaude) {
    if (!e->vivant || !texJeanClaude) return;

    int texW, texH;
    SDL_QueryTexture(texJeanClaude, NULL, NULL, &texW, &texH);

    int nbFrames = 3;
    int frameW = texW / 3;
    int frameH = texH / 4;
    int currentFrame = (SDL_GetTicks() / 300) % nbFrames;
    int ligne = (e->direction > 0) ? 1 : 3;

    SDL_Rect src = { currentFrame * frameW, ligne * frameH, frameW, frameH };
    int displayW = 32; 
    int displayH = 48; 
    int offsetX = (e->rect.w - displayW) / 2;
    int offsetY = e->rect.h - displayH; 

    SDL_Rect dest = { 
        e->rect.x - scrollX + offsetX, 
        e->rect.y - scrollY + offsetY, 
        displayW, 
        displayH 
    };
    SDL_RenderCopy(renderer, texJeanClaude, &src, &dest);

}


//==============================================================
//============================Olaf==============================
//==============================================================

// Dans ennemi.c

void init_snowman(Ennemi* e, int x, int y) {
    e->rect.x = x; 
    e->rect.y = y;
    e->rect.w = 28; 
    e->rect.h = 32; 
    e->velY = 0.0f; 
    e->speed = 1.5f; 
    e->direction = -1; 
    e->onGround = 0;
    e->vivant = 1;
}

void update_snowman(Ennemi* e, int* map, int levelID) {
    update_loupas(e, map, levelID); 
}

void render_snowman(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, SDL_Texture* texOlaf) {
    if (!e->vivant || !texOlaf) return;

    int texW, texH;
    SDL_QueryTexture(texOlaf, NULL, NULL, &texW, &texH);
    int nbFrames = 4; 
    int frameW = texW / nbFrames;
    int frame = (SDL_GetTicks() / 150) % nbFrames;
    SDL_Rect src = { frame * frameW, 0, frameW, texH };
    int tailleImage = 64;
    int posX = (e->rect.x + e->rect.w / 2) - (tailleImage / 2);
    int posY = (e->rect.y + e->rect.h) - tailleImage;

    posY += 15; 
    SDL_Rect dest = { 
        posX - scrollX, 
        posY - scrollY, 
        tailleImage, 
        tailleImage 
    };

    if (e->direction > 0) {
        SDL_RenderCopyEx(renderer, texOlaf, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderCopy(renderer, texOlaf, &src, &dest);
    }
}


//==============================================================
//======================Presse Hydraulique======================
//==============================================================
#define PRESSE_FRAME_W 32 
#define PRESSE_FRAME_H 32 
#define NUM_PRESSE_FRAMES 5
#define PRESSE_ANIM_DELAY 100 

void init_presse(Presse* p, int x, int y, Uint32 offset) {
    p->baseRect = (SDL_Rect){x, y, 50, 64}; 
    p->extensionRect = (SDL_Rect){x, y, 50, 30};
    
    p->currentFrame = 0;
    p->frameTimer = SDL_GetTicks();
    p->startDelay = offset; // On enregistre le décalage
    p->activated = (offset == 0); // Si offset est 0, on l'active direct
    p->animationState = 0;
    p->vivant = 1;
}

void update_presse(Presse* p) {
    if (!p->vivant) return;

    // Gestion du délai de l'animation pour chaque presse 
    if (!p->activated) {
        if (SDL_GetTicks() - p->frameTimer > p->startDelay) {
            p->activated = 1;
            p->frameTimer = SDL_GetTicks();
        }
        return; 
    }

    // Délais d'animation
    int delay = 60; 
    if (p->currentFrame == 0) delay = 700; // Pause en haut
    if (p->currentFrame == 2) delay = 300; // Pause en bas 

    if (SDL_GetTicks() - p->frameTimer > (Uint32)delay) {
        if (p->animationState == 0) { //chute
            if (p->currentFrame < 2) p->currentFrame++;
            else p->animationState = 1;
        } 
        else { //remontée
            if (p->currentFrame > 0) p->currentFrame--;
            else p->animationState = 0;
        }

        // Hitbox adaptative avec les frames de la presse
        int h[] = {50, 60, 80}; 
        p->extensionRect.h = h[p->currentFrame];
        p->frameTimer = SDL_GetTicks();
    }
}

void render_presse(SDL_Renderer* renderer, Presse* p, int camX, int camY, SDL_Texture* tex) {
    if (!p->vivant || !tex) return;

    int texW, texH;
    SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);
    int frameW = texW / NUM_PRESSE_FRAMES; 
    int frameH = texH; 

    SDL_Rect src = { p->currentFrame * frameW, 0, frameW, frameH };
    SDL_Rect dest = { 
        p->baseRect.x - camX, 
        p->baseRect.y - camY, 
        p->baseRect.w, 96  
    };

    SDL_RenderCopy(renderer, tex, &src, &dest);
}