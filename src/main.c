#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h> // Ne pas oublier cet include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusion de nos headers persos
#include "map.h"
#include "player.h"
#include "camera.h"
#include "defs.h"
#include "menu.h" // Inclusion du menu

#define TILE_SIZE 16      
#define MAP_SCALE 2       
#define MAP_WIDTH 270  
#define MAP_HEIGHT 38  
#define REFERENCE_HEIGHT 600 
#define TILE_SOLID 7

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // --- 1. Initialisation Globale (SDL + IMG + TTF) ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    IMG_Init(IMG_INIT_PNG);
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    // Création de la fenêtre et du renderer UNE SEULE FOIS
    SDL_Window* window = SDL_CreateWindow("Jeu Mario Like Modularise", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    if (!window) {
        printf("Erreur creation fenetre\n");
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Récupérer la taille réelle de l'écran pour le menu
    int physicalW, physicalH;
    SDL_GetRendererOutputSize(renderer, &physicalW, &physicalH);

    // --- 2. Lancement du Menu ---
    // On passe le renderer au menu. Le menu gère sa propre boucle.
    MenuResult menuChoix = afficher_menu(renderer, physicalW, physicalH);

    // Si le joueur a cliqué sur Quitter ou fermé la fenêtre dans le menu
    if (menuChoix == MENU_QUIT) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 0;
    }

    // --- 3. Configuration Jeu (S'il a cliqué JOUER) ---
    
    // IMPORTANT : On remet la "Logical Size" pour le Pixel Art du jeu
    float aspectRatio = (float)physicalW / (float)physicalH;
    int logicalH = REFERENCE_HEIGHT;
    int logicalW = (int)(REFERENCE_HEIGHT * aspectRatio);
    SDL_RenderSetLogicalSize(renderer, logicalW, logicalH);

    // Initialisation Map
    int* tileMap = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
    if (!load_map_from_csv("assets/Maps/map_v1.3.csv", tileMap)) {
        free(tileMap);
        // Nettoyage rapide avant de partir
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
        return 1;
    }

    // --- 4. Chargement Assets Jeu ---
    SDL_Texture* terrainTex = IMG_LoadTexture(renderer, "assets/Terrain/Terrain (16x16).png");
    Tileset ts = { terrainTex, 22, 16, 16 };
    
    SDL_Texture* bg1 = IMG_LoadTexture(renderer, "assets/Sprites/Background/sky.png");
    SDL_Texture* bg2 = IMG_LoadTexture(renderer, "assets/Sprites/Background/mountain2.png");
    SDL_Texture* bg3 = IMG_LoadTexture(renderer, "assets/Sprites/Background/pine1.png");
    SDL_Texture* bg4 = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud1.png");
    SDL_Texture* bg5 = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud2.png");

    SDL_Texture* texIdle = IMG_LoadTexture(renderer, "assets/Personnage/Idle.png");
    SDL_Texture* texRun  = IMG_LoadTexture(renderer, "assets/Personnage/Run.png");
    SDL_Texture* texJump = IMG_LoadTexture(renderer, "assets/Personnage/Jump.png");

    // --- 5. Initialisation Objets ---
    Player player;
    init_player(&player, 100, 100);

    Camera camera;
    camera.w = logicalW;
    camera.h = logicalH;
    camera.x = 0;
    camera.y = 0;

    int mapPixelWidth = MAP_WIDTH * TILE_SIZE * MAP_SCALE;
    int mapPixelHeight = MAP_HEIGHT * TILE_SIZE * MAP_SCALE;

    // --- 6. Boucle Principale du Jeu ---
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) 
                running = 0;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        update_player(&player, keys, tileMap);
        update_camera(&camera, &player, mapPixelWidth, mapPixelHeight);

        SDL_RenderClear(renderer);

        draw_parallax_bg(renderer, bg1, camera.x, camera.y, 0.05f, 0.0f, logicalW, logicalH, 0); 
        draw_parallax_bg(renderer, bg4, camera.x, camera.y, 0.08f, 0.02f, logicalW, logicalH, 50);
        draw_parallax_bg(renderer, bg5, camera.x, camera.y, 0.10f, 0.03f, logicalW, logicalH, 150);
        draw_parallax_bg(renderer, bg2, camera.x, camera.y, 0.15f, 0.05f, logicalW, logicalH, 200); 
        draw_parallax_bg(renderer, bg3, camera.x, camera.y, 0.40f, 0.1f, logicalW, logicalH, 350);

        for (int y = 0; y < MAP_HEIGHT; y++) { 
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tilePixelX = x * TILE_SIZE * MAP_SCALE;
                if (tilePixelX > camera.x - 64 && tilePixelX < camera.x + logicalW + 64) {
                    draw_tile(renderer, &ts, tileMap[y * MAP_WIDTH + x], tilePixelX, y * TILE_SIZE * MAP_SCALE, camera.x, camera.y);
                }
            }
        }

        render_player(renderer, &player, camera.x, camera.y, texIdle, texRun, texJump);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // --- 7. Nettoyage Final ---
    free(tileMap);
    SDL_DestroyTexture(bg1); SDL_DestroyTexture(bg2); SDL_DestroyTexture(bg3); 
    SDL_DestroyTexture(bg4); SDL_DestroyTexture(bg5); SDL_DestroyTexture(terrainTex); 
    SDL_DestroyTexture(texIdle); SDL_DestroyTexture(texRun); SDL_DestroyTexture(texJump);
    
    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(window);
    
    TTF_Quit(); // Important de quitter TTF
    IMG_Quit();
    SDL_Quit();
    return 0;
}