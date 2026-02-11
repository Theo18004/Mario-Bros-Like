#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h> 
#include "map.h"
#include "player.h"

// Fonction utilitaire pour charger une texture proprement
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path);
    if (!tex) {
        printf("ERREUR : Impossible de charger l'image %s ! Erreur : %s\n", path, IMG_GetError());
    }
    return tex;
}

int main(int argc, char* argv[]) {
    // --- 1. INITIALISATION SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Mario Bros Like - SDL2", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    
    // On active la VSYNC pour éviter que le jeu tourne trop vite
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // --- 2. INITIALISATION JOUEUR ---
    Player player = {
        .rect = {0, 560, 40, 60}, // Hitbox physique (x, y, largeur, hauteur)
        .velocityY = 0.0f,
        .isJumping = 0,
        .animState = ANIM_IDLE,
        .facingLeft = 0,
        .frameTimer = 0,
        .frameIndex = 0
    };

    // Chargement des Textures
    // Assurez-vous d'avoir le dossier "assets" à côté de l'exécutable
    player.texIdle = loadTexture(renderer, "assets/Idle.png");
    player.texRun  = loadTexture(renderer, "assets/Run.png");
    player.texJump = loadTexture(renderer, "assets/Jump.png");

    // Configuration du nombre de frames par animation (selon vos images)
    const int FRAMES_IDLE = 6;
    const int FRAMES_RUN  = 8;
    const int FRAMES_JUMP = 10;

    // --- 3. CHARGEMENT MAP ---
    SDL_Rect* worldPlatforms = NULL;
    int platformCount = 0;
    
    // On passe l'adresse du rect du joueur pour définir sa position de départ via le carré bleu de la map
    loadMapFromImage("assets/map.png", &worldPlatforms, &platformCount, &player.rect);
    SDL_Texture* mapBackground = loadTexture(renderer, "assets/map.png");

    // --- 4. BOUCLE PRINCIPALE ---
    int running = 1;
    SDL_Event e;
    
    while (running) {
        // A. Gestion des événements
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        // B. Mise à jour (Update)
        updatePlayer(&player); 

        // C. Affichage (Render)
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Bleu Ciel
        SDL_RenderClear(renderer);

        // 1. Fond
        if (mapBackground) {
            SDL_RenderCopy(renderer, mapBackground, NULL, NULL);
        }

        // 2. Plateformes
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < platformCount; i++) {
            SDL_RenderFillRect(renderer, &worldPlatforms[i]);
        }

        // 3. Dessin du Joueur (Sprite)
        SDL_Texture* currentTex = player.texIdle;
        int maxFrames = FRAMES_IDLE;

        // Choix de la texture selon l'état
        switch (player.animState) {
            case ANIM_RUN:
                currentTex = player.texRun;
                maxFrames = FRAMES_RUN;
                break;
            case ANIM_JUMP:
                currentTex = player.texJump;
                maxFrames = FRAMES_JUMP;
                break;
            default:
                currentTex = player.texIdle;
                maxFrames = FRAMES_IDLE;
                break;
        }

        if (currentTex != NULL) {
            // -- Calcul du découpage de l'image (Source) --
            int texW, texH;
            SDL_QueryTexture(currentTex, NULL, NULL, &texW, &texH);
            
            // La largeur d'une frame = Largeur totale image / Nombre de frames
            int frameWidth = texW / maxFrames; 
            
            // Quelle frame afficher ? (Modulo pour boucler : 0, 1, 2... max-1, 0...)
            int currentFrameIndex = (int)player.frameTimer % maxFrames;

            SDL_Rect srcRect = {
                .x = currentFrameIndex * frameWidth,
                .y = 0,
                .w = frameWidth,
                .h = texH
            };

            // -- Calcul de la position sur l'écran (Destination) --
            // Le sprite est souvent plus grand que la hitbox physique.
            // On centre le sprite horizontalement sur la hitbox.
            // On aligne le bas du sprite avec le bas de la hitbox (les pieds).
            
            // On peut grossir le sprite (scale) si nécessaire. Ici on garde la taille originale (x1.5 par exemple)
            float scale = 1.5f; 
            int displayW = (int)(frameWidth * scale);
            int displayH = (int)(texH * scale);

            SDL_Rect destRect = {
                .x = player.rect.x - (displayW - player.rect.w) / 2, // Centrage horizontal
                .y = player.rect.y - (displayH - player.rect.h),     // Alignement par le bas
                .w = displayW,
                .h = displayH
            };

            // Gestion de la direction (Flip)
            SDL_RendererFlip flip = player.facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            // Dessin final
            SDL_RenderCopyEx(renderer, currentTex, &srcRect, &destRect, 0.0, NULL, flip);
        }



        SDL_RenderPresent(renderer);
    }

    // --- 5. NETTOYAGE ---
    if (worldPlatforms) free(worldPlatforms);
    if (player.texIdle) SDL_DestroyTexture(player.texIdle);
    if (player.texRun)  SDL_DestroyTexture(player.texRun);
    if (player.texJump) SDL_DestroyTexture(player.texJump);
    if (mapBackground)  SDL_DestroyTexture(mapBackground);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}