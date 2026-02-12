#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "player.h"
#include "camera.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Supprime les warnings unused parameter

    // --- 1. INITIALISATION DES DONNÉES ---
    int* tileMap = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
    
    // Charge la map depuis le fichier CSV. Si ça échoue, affiche une erreur et quitte.
    if (!load_map_from_csv("assets/map.csv", tileMap)) { 
        printf("Erreur : Impossible de charger assets/map.csv.\n");
        free(tileMap);
        return 1;
    }

    // --- 2. INITIALISATION SDL ---
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Pixel Adventure", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int screenW, screenH;
    SDL_GetRendererOutputSize(renderer, &screenW, &screenH);

    // --- 3. INITIALISATION DU JOUEUR ET ASSETS ---
    Player player = {
        .rect = {100, 200, 30, 45}, 
        .velocityY = 0.0f,
        .isJumping = 0,
        .animState = ANIM_IDLE,
        .facingLeft = 0,
        .frameTimer = 0.0f
    };

    player.texIdle = IMG_LoadTexture(renderer, "assets/Idle.png");
    player.texRun  = IMG_LoadTexture(renderer, "assets/Run.png");
    player.texJump = IMG_LoadTexture(renderer, "assets/Jump.png");

    SDL_Texture* terrainTex = IMG_LoadTexture(renderer, "assets/Terrain.png");
    Tileset ts = { terrainTex, 22, 16, 16 };

    Camera cam = {0, 0, screenW, screenH};

    // --- 4. BOUCLE PRINCIPALE ---
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
               (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }

        // --- A. MISE À JOUR (player.c) ---
        // Utilise tes fonctions avancer/sauter via updatePlayer
        updatePlayer(&player, NULL, 0, screenH);

        // --- B. CAMÉRA ---
        updateCamera(&cam, player.rect, screenW, screenH);

        // --- C. RENDU ---
        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255); 
        SDL_RenderClear(renderer);

        // 1. Dessin de la Map
        int mapTotalHeight = MAP_HEIGHT * TILE_SIZE * MAP_SCALE;
        int scrollY = mapTotalHeight - screenH; 
        render_world(renderer, tileMap, &ts, cam.x, scrollY);

        // 2. Dessin du Joueur Animé
        SDL_Texture* curTex = player.texIdle;
        int maxFrames = 6; 
        if (player.animState == ANIM_RUN) { curTex = player.texRun; maxFrames = 8; }
        else if (player.animState == ANIM_JUMP) { curTex = player.texJump; maxFrames = 10; }

        if (curTex) {
            int tw, th;
            SDL_QueryTexture(curTex, NULL, NULL, &tw, &th);
            int fw = tw / maxFrames;
            SDL_Rect src = { ((int)player.frameTimer % maxFrames) * fw, 0, fw, th };
            
            float scale = 0.8f;
            int dw = (int)(fw * scale), dh = (int)(th * scale);
            SDL_Rect dst = { 
                player.rect.x - (dw - player.rect.w) / 2 - cam.x, 
                player.rect.y - (dh - player.rect.h) - scrollY + 1, 
                dw, dh 
            };

            SDL_RendererFlip flip = player.facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            SDL_RenderCopyEx(renderer, curTex, &src, &dst, 0, NULL, flip);
        }

        SDL_RenderPresent(renderer);
    }

    // --- 5. NETTOYAGE ---
    free(tileMap);
    SDL_DestroyTexture(terrainTex);
    SDL_DestroyTexture(player.texIdle);
    SDL_DestroyTexture(player.texRun);
    SDL_DestroyTexture(player.texJump);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}