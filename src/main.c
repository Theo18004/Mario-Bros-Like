/**
 * @file main.c
 * @brief Point d'entrée principal du programme.
 * @details Initialise SDL2, charge les ressources, gère la boucle de jeu.
 */

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
#include "ennemi.h" // Nouveau header pour les ennemis

#define TILE_SIZE 16      
#define MAP_SCALE 2       
#define MAP_WIDTH 270  
#define MAP_HEIGHT 38  
#define REFERENCE_HEIGHT 600 
#define TILE_SOLID 7

/**
 * @brief Fonction main.
 */

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // --- 1. Initialisation Globale ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    IMG_Init(IMG_INIT_PNG);
    if (TTF_Init() == -1) return 1;

    SDL_Window* window = SDL_CreateWindow("Mario-Like (Ennemis Rouges)", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int physicalW, physicalH;
    SDL_GetRendererOutputSize(renderer, &physicalW, &physicalH);

    // charger parametre sauvegarder
    charger_parametres();

    // --- 2. Lancement du Menu ---
    MenuResult menuChoix = afficher_menu(renderer, physicalW, physicalH);
    if (menuChoix == MENU_QUIT) {
        // Nettoyage rapide
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
        TTF_Quit(); IMG_Quit(); SDL_Quit();
        return 0;
    }

    // --- 3. Configuration Logique ---
    float aspectRatio = (float)physicalW / (float)physicalH;
    int logicalH = REFERENCE_HEIGHT;
    int logicalW = (int)(REFERENCE_HEIGHT * aspectRatio);
    SDL_RenderSetLogicalSize(renderer, logicalW, logicalH);

    // Initialisation Map
    int* tileMap = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
    if (!load_map_from_csv("assets/Maps/map_v3.csv", tileMap)) {
        free(tileMap);
        return 1;
    }

    // --- 4. Chargement Assets ---
    SDL_Texture* terrainTex = IMG_LoadTexture(renderer, "assets/Terrain/CustomTileset.png");
    Tileset ts = { terrainTex, 22, 16, 16 };
    
    // Backgrounds
    SDL_Texture* bg1 = IMG_LoadTexture(renderer, "assets/Sprites/Background/sky.png");
    SDL_Texture* bg2 = IMG_LoadTexture(renderer, "assets/Sprites/Background/mountain2.png");
    SDL_Texture* bg3 = IMG_LoadTexture(renderer, "assets/Sprites/Background/pine1.png");
    SDL_Texture* bg4 = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud1.png");
    SDL_Texture* bg5 = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud2.png");

    // Textures Joueur (Réutilisées pour l'ennemi)
    SDL_Texture* texIdle = IMG_LoadTexture(renderer, "assets/Personnage/Idle.png");
    SDL_Texture* texRun  = IMG_LoadTexture(renderer, "assets/Personnage/Run.png");
    SDL_Texture* texJump = IMG_LoadTexture(renderer, "assets/Personnage/Jump.png");
    SDL_Texture* texDead = IMG_LoadTexture(renderer, "assets/Personnage/Dead.png");
    SDL_Texture* texFullHeart = IMG_LoadTexture(renderer, "assets/Interface/FullHeart.png");

    // --- 5. Initialisation Objets ---
    Player player;
    init_player(&player, 20, 1000); // Position de départ
    player.lives = 3;

    // Création d'un ennemi
    Ennemi mechant;
    init_ennemi(&mechant, 600, 1000); // Placé plus loin sur la carte

    Score score;
    init_score(&score, 10, 10);

    Camera camera;
    camera.w = logicalW;
    camera.h = logicalH;
    camera.x = 0;
    camera.y = 0;

    int mapPixelWidth = MAP_WIDTH * TILE_SIZE * MAP_SCALE;
    int mapPixelHeight = MAP_HEIGHT * TILE_SIZE * MAP_SCALE;

    // --- 6. Boucle Principale ---
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) 
                running = 0;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        // -- Mises à jour --
        update_player(&player, keys, tileMap, &score);
        update_ennemi(&mechant, tileMap);
        update_score(&score, (int)player.rect.x);
        update_camera(&camera, &player, mapPixelWidth, mapPixelHeight);

        // -- Gestion Collision Joueur/Ennemi --
        if (mechant.vivant && SDL_HasIntersection(&player.rect, &mechant.rect)) {
            // Si le joueur tombe sur l'ennemi (vitesse Y positive et au-dessus)
            if (player.velY > 0 && player.rect.y + player.rect.h < mechant.rect.y + 20) {
                mechant.vivant = 0;
                player.velY = -10.0f; // Petit rebond
            } else if (player.state != STATE_DEAD) {
                // Le joueur touche l'ennemi de côté : il meurt ou perd une vie
                gerer_mort_joueur(&player, 20, 1000, &score);
            }
        }

        // -- Rendu --
        SDL_SetRenderDrawColor(renderer, 27, 45, 45, 255);
        SDL_RenderClear(renderer);

        // Parallax
        draw_parallax_bg(renderer, bg1, camera.x, camera.y, 0.05f, 0.0f, logicalW, logicalH, 0); 
        draw_parallax_bg(renderer, bg4, camera.x, camera.y, 0.08f, 0.02f, logicalW, logicalH, 50);
        draw_parallax_bg(renderer, bg5, camera.x, camera.y, 0.10f, 0.03f, logicalW, logicalH, 150);
        draw_parallax_bg(renderer, bg2, camera.x, camera.y, 0.15f, 0.05f, logicalW, logicalH, 200); 
        draw_parallax_bg(renderer, bg3, camera.x, camera.y, 0.40f, 0.1f, logicalW, logicalH, 350);

        // Dessin de la Map
        for (int y = 0; y < MAP_HEIGHT; y++) { 
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tilePixelX = x * TILE_SIZE * MAP_SCALE;
                if (tilePixelX > camera.x - 64 && tilePixelX < camera.x + logicalW + 64) {
                    draw_tile(renderer, &ts, tileMap[y * MAP_WIDTH + x], tilePixelX, y * TILE_SIZE * MAP_SCALE, camera.x, camera.y);
                }
            }
        }

        // Entités
        render_ennemi(renderer, &mechant, camera.x, camera.y, texIdle, texRun);
        render_player(renderer, &player, camera.x, camera.y, texIdle, texRun, texJump, texDead);

        // Interface
        render_score(renderer, &score);
        render_lives(renderer, texFullHeart, player.lives);
        render_progress_bar(renderer, player.rect.x, mapPixelWidth);

        // --- EFFET LUMINOSITÉ ---
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite); 
        SDL_Rect fullScreen = {0, 0, physicalW, physicalH};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // --- 7. Nettoyage ---
    free(tileMap);
    SDL_DestroyTexture(bg1); SDL_DestroyTexture(bg2); SDL_DestroyTexture(bg3); 
    SDL_DestroyTexture(bg4); SDL_DestroyTexture(bg5); SDL_DestroyTexture(terrainTex); 
    SDL_DestroyTexture(texIdle); SDL_DestroyTexture(texRun); SDL_DestroyTexture(texJump); SDL_DestroyTexture(texDead);
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();

    return 0;
}