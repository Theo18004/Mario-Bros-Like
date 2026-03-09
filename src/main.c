#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusion de nos headers persos
#include "map.h"
#include "player.h"
#include "camera.h"
#include "defs.h"
#include "menu.h"
#include "score.h"
#include "mort.h"
#include "interface.h"

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
    SDL_Window* window = SDL_CreateWindow("Mario Bros Like", 
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

    // --- 3. Configuration Jeu  ---
    
    // IMPORTANT : On remet la "Logical Size" pour le Pixel Art du jeu
    float aspectRatio = (float)physicalW / (float)physicalH;
    int logicalH = REFERENCE_HEIGHT;
    int logicalW = (int)(REFERENCE_HEIGHT * aspectRatio);
    SDL_RenderSetLogicalSize(renderer, logicalW, logicalH);

    // Initialisation Map
    int* tileMap = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
    if (!load_map_from_csv("assets/Maps/map_v3.csv", tileMap)) {
        free(tileMap);
        // Nettoyage rapide avant de partir
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
        return 1;
    }

    // --- 4. Chargement Assets Jeu ---
    SDL_Texture* terrainTex = IMG_LoadTexture(renderer, "assets/Terrain/CustomTileset.png");
    Tileset ts = { terrainTex, 22, 16, 16 };
    
    SDL_Texture* bg1 = IMG_LoadTexture(renderer, "assets/Sprites/Background/sky.png");
    SDL_Texture* bg2 = IMG_LoadTexture(renderer, "assets/Sprites/Background/mountain2.png");
    SDL_Texture* bg3 = IMG_LoadTexture(renderer, "assets/Sprites/Background/pine1.png");
    SDL_Texture* bg4 = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud1.png");
    SDL_Texture* bg5 = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud2.png");
    SDL_Texture* bg6 = IMG_LoadTexture(renderer, "assets/Sprites/Background/Donjon.png");

    SDL_Texture* texIdle = IMG_LoadTexture(renderer, "assets/Personnage/Idle.png");
    SDL_Texture* texRun  = IMG_LoadTexture(renderer, "assets/Personnage/Run.png");
    SDL_Texture* texJump = IMG_LoadTexture(renderer, "assets/Personnage/Jump.png");
    SDL_Texture* texDead = IMG_LoadTexture(renderer, "assets/Personnage/Dead.png");
    SDL_Texture* texFullHeart = IMG_LoadTexture(renderer, "assets/Interface/FullHeart.png");
    //SDL_Texture* texEmptyHeart = IMG_LoadTexture(renderer, "assets/Interface/BreakHeart.png");  A VOIR SI ON LE MET OU PAS

    // --- 5. Initialisation Objets ---
    Player player;
    init_player(&player, 20, 1000); // Position de départ du joueur sur la terre
    player.lives = 3; // Initialisation du nombre de vies

    Score score;
    init_score(&score, 10, 10); // Position en haut à gauche

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
        update_player(&player, keys, tileMap, &score);
        update_score(&score, (int)player.rect.x);
        update_camera(&camera, &player, mapPixelWidth, mapPixelHeight);

        SDL_SetRenderDrawColor(renderer, 27, 45, 45, 255);
        SDL_RenderClear(renderer);

        draw_parallax_bg(renderer, bg1, camera.x, camera.y, 0.05f, 0.0f, logicalW, logicalH, 0, 0); 
        draw_parallax_bg(renderer, bg4, camera.x, camera.y, 0.08f, 0.02f, logicalW, logicalH, 0, 50);
        draw_parallax_bg(renderer, bg5, camera.x, camera.y, 0.10f, 0.03f, logicalW, logicalH, 0, 150);
        draw_parallax_bg(renderer, bg2, camera.x, camera.y, 0.15f, 0.05f, logicalW, logicalH, 0, 200); 
        draw_parallax_bg(renderer, bg3, camera.x, camera.y, 0.40f, 0.1f, logicalW, logicalH, 0, 350);
       
        int startDonjonX = 128 * 32; // Début (Tuile 128)
        int endDonjonX = 317 * 32;   // Fin (Tuile 317)

        int xStartEcran = startDonjonX - camera.x;
        int xEndEcran = endDonjonX - camera.x;

        // On définit le clip pour qu'il s'arrête à xEndEcran
        SDL_Rect clip;
        clip.x = xStartEcran;
        clip.y = 0;
        clip.w = xEndEcran - xStartEcran; // La largeur est la distance entre le début et la fin
        clip.h = logicalH;

        SDL_RenderSetClipRect(renderer, &clip);
        draw_parallax_bg(renderer, bg6, camera.x, camera.y, 0.15f, 0.0f, logicalW, logicalH, startDonjonX, -300);
        SDL_RenderSetClipRect(renderer, NULL);



        for (int y = 0; y < MAP_HEIGHT; y++) { 
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tilePixelX = x * TILE_SIZE * MAP_SCALE;
                if (tilePixelX > camera.x - 64 && tilePixelX < camera.x + logicalW + 64) {
                    draw_tile(renderer, &ts, tileMap[y * MAP_WIDTH + x], tilePixelX, y * TILE_SIZE * MAP_SCALE, camera.x, camera.y);
                }
            }
        }

        render_player(renderer, &player, camera.x, camera.y, texIdle, texRun, texJump, texDead);
        render_score(renderer, &score);
        render_lives(renderer, texFullHeart, player.lives);
        render_progress_bar(renderer, player.rect.x, mapPixelWidth);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // --- 7. Nettoyage Final ---
    free(tileMap);
    SDL_DestroyTexture(bg1); SDL_DestroyTexture(bg2); SDL_DestroyTexture(bg3); 
    SDL_DestroyTexture(bg4); SDL_DestroyTexture(bg5); SDL_DestroyTexture(bg6); SDL_DestroyTexture(terrainTex); 
    SDL_DestroyTexture(texIdle); SDL_DestroyTexture(texRun); SDL_DestroyTexture(texJump); SDL_DestroyTexture(texDead);
    
    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(window);
    
    TTF_Quit(); 
    IMG_Quit();
    SDL_Quit();
    return 0;
}