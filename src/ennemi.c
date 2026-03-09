#include "ennemi.h"
#include "collision.h"
#include "defs.h"

void init_ennemi(Ennemi* e, int x, int y) {
    e->rect.x = x;
    e->rect.y = y;
    e->rect.w = 30;    // Même largeur que le joueur
    e->rect.h = 48;    // Même hauteur que le joueur
    e->velY = 0.0f;
    e->speed = 2.0f;   // Vitesse de patrouille
    e->direction = 1;  // Commence par aller à droite
    e->onGround = 0;
    e->state = STATE_RUN;
    e->vivant = 1;
}

void update_ennemi(Ennemi* e, int* map) {
    if (!e->vivant) return;

    int oldX = e->rect.x;
    int tileSize = TILE_SIZE * MAP_SCALE;
    
    // 1. Déplacement horizontal
    e->rect.x += (int)(e->speed * e->direction);

    // 2. Détection des murs (collision horizontale classique)
    if (check_collision(e->rect, map)) {
        e->rect.x = oldX;
        e->direction *= -1;
    }

    // 3. DÉTECTION DU VIDE (Empêcher de tomber)
    // On crée un petit rectangle de test juste devant l'ennemi et un peu plus bas
    SDL_Rect testVide = e->rect;
    testVide.y += 5; // On regarde un peu sous ses pieds
    
    // On décale le capteur vers la gauche ou la droite selon la direction
    if (e->direction > 0) {
        testVide.x += e->rect.w; // Capteur à droite
    } else {
        testVide.x -= 5; // Capteur à gauche
    }
    testVide.w = 5; // Largeur du capteur

    // Si le capteur NE TOUCHE PAS le sol, c'est qu'il y a un trou
    if (!check_collision(testVide, map) && e->onGround) {
        e->rect.x = oldX;    // On annule le mouvement
        e->direction *= -1; // On fait demi-tour
    }

    // 4. Physique Y (Gravité habituelle)
    e->velY += 0.6f;
    e->rect.y += (int)e->velY;

    if (check_collision(e->rect, map)) {
        e->rect.y = ((e->rect.y + e->rect.h) / tileSize) * tileSize - e->rect.h;
        e->velY = 0;
        e->onGround = 1;
    } else {
        e->onGround = 0;
    }
}

void render_ennemi(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, 
                   SDL_Texture* texIdle, SDL_Texture* texRun) {
    if (!e->vivant) return;

    // --- ASTUCE : Teinte Rouge ---
    // On applique un filtre rouge (R=255, G=100, B=100 pour garder un peu de détail)
    SDL_SetTextureColorMod(texRun, 255, 100, 100);
    SDL_SetTextureColorMod(texIdle, 255, 100, 100);

    // On réutilise la même logique que ton render_player
    SDL_Texture* currentTexture = (e->direction != 0) ? texRun : texIdle;
    int nbFrames = (currentTexture == texRun) ? 8 : 6;
    
    int texW, texH;
    SDL_QueryTexture(currentTexture, NULL, NULL, &texW, &texH);
    int singleFrameW = texW / nbFrames;
    int currentFrame = (SDL_GetTicks() / 120) % nbFrames;

    SDL_Rect src = { currentFrame * singleFrameW, 0, singleFrameW, texH };
    SDL_Rect dest = { e->rect.x - scrollX - 17, e->rect.y - scrollY - 16, 64, 64 };
    
    SDL_RendererFlip flip = (e->direction > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, currentTexture, &src, &dest, 0, NULL, flip);

    // --- IMPORTANT : On remet la texture normale pour le joueur ---
    SDL_SetTextureColorMod(texRun, 255, 255, 255);
    SDL_SetTextureColorMod(texIdle, 255, 255, 255);
}