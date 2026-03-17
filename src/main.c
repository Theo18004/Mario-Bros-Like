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
#include "ennemi.h" 
#include "Items.h"

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

    TTF_Font* font = TTF_OpenFont("assets/police.ttf", 32); 
    if (font == NULL) {
        printf("Erreur : impossible de charger la police ! %s\n", TTF_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("Mario-Like (Loup & Thwomp)", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int physicalW, physicalH;
    SDL_GetRendererOutputSize(renderer, &physicalW, &physicalH);

    // charger parametre sauvegarder
    charger_parametres();

    // --- 2. Lancement du Menu ---
    MenuResult menuChoix = afficher_menu(renderer, physicalW, physicalH);
    if (menuChoix == MENU_QUIT) {
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
    int* tileMap = (int*)malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
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
    SDL_Texture* bg6 = IMG_LoadTexture(renderer, "assets/Sprites/Background/Donjon.png");
    SDL_Texture* bg7 = IMG_LoadTexture(renderer, "assets/Sprites/Background/desert2.png");
    SDL_Texture* texStatue = IMG_LoadTexture(renderer, "assets/Sprites/Background/statue.png");
    SDL_Texture* texPyramide = IMG_LoadTexture(renderer, "assets/Sprites/Background/pyramide.png");
    SDL_Texture* texChateau = IMG_LoadTexture(renderer, "assets/Sprites/Background/chateau.png");

    // Textures Joueur
    SDL_Texture* texIdle = IMG_LoadTexture(renderer, "assets/Personnage/Idle.png");
    SDL_Texture* texRun  = IMG_LoadTexture(renderer, "assets/Personnage/Run.png");
    SDL_Texture* texJump = IMG_LoadTexture(renderer, "assets/Personnage/Jump.png");
    SDL_Texture* texDead = IMG_LoadTexture(renderer, "assets/Personnage/Dead.png");
    SDL_Texture* texFullHeart = IMG_LoadTexture(renderer, "assets/Interface/FullHeart.png");

    // Textures Ennemis
    SDL_Texture* texLoup = IMG_LoadTexture(renderer, "assets/Ennemi/wolfsheet.png");
    SDL_Texture* texThwomp = IMG_LoadTexture(renderer, "assets/Ennemi/thwompEdit.png");
    SDL_Texture* texPodoboo = IMG_LoadTexture(renderer, "assets/Ennemi/podoboo.png");

    //Textures Items
    SDL_Texture* texCoin = IMG_LoadTexture(renderer, "assets/Items/coin50.png");
    SDL_Texture* texCheckpoint = IMG_LoadTexture(renderer, "assets/Items/Checkpoint.png");

    // --- 5. Initialisation Objets ---
    Player player;
    init_player(&player, 20, 1000); //14176
    player.lives = 3;

    // Création du Loup
    Ennemi mechant;
    init_loupas(&mechant, 600, 1000);

    // Création du Thwomp
    #define NB_THWOMPS 8
    Thwomp mesThwomps[NB_THWOMPS];
    init_thwomp(&mesThwomps[0], 8960, 672);
    init_thwomp(&mesThwomps[1], 9120, 672);
    init_thwomp(&mesThwomps[2], 9250, 672);
    init_thwomp(&mesThwomps[3], 9340, 672);
    init_thwomp(&mesThwomps[4], 9400, 672);
    init_thwomp(&mesThwomps[5], 9568, 672);
    init_thwomp(&mesThwomps[6], 9728, 672);
    init_thwomp(&mesThwomps[7], 9888, 672);

    //Création des Podoboo
    Podoboo mesPodoboo[NB_PODOBOO];
    init_podoboo(&mesPodoboo[0], 7296, 1088, 800); 
    init_podoboo(&mesPodoboo[1], 7488, 1088, 800);
    init_podoboo(&mesPodoboo[2], 7648, 1088, 800); 
    init_podoboo(&mesPodoboo[3], 7680, 1088, 800);
    init_podoboo(&mesPodoboo[4], 7776, 1088, 800); 
    init_podoboo(&mesPodoboo[5], 7840, 1088, 800);
    init_podoboo(&mesPodoboo[6], 7968, 1088, 800); 
    init_podoboo(&mesPodoboo[7], 8128, 1088, 800);
    init_podoboo(&mesPodoboo[8], 8192, 1088, 800); 
    init_podoboo(&mesPodoboo[9], 8256, 1088, 800);
    init_podoboo(&mesPodoboo[10], 8320, 1088, 800); 
   
    // Création des pièces
    Piece mesPieces[NB_PIECES];
    for (int i = 0; i < NB_PIECES; i++) {
        mesPieces[i].rect.w = 32;
        mesPieces[i].rect.h = 32;
        mesPieces[i].vivant = 1;
    }
    mesPieces[0].rect.x = 1088; mesPieces[0].rect.y = 1024;
    mesPieces[1].rect.x = 2144; mesPieces[1].rect.y = 444;
    mesPieces[2].rect.x = 4352; mesPieces[2].rect.y = 576;
    mesPieces[3].rect.x = 11585; mesPieces[3].rect.y = 640;

    // Création des checkpoints
    Checkpoint mesCheckpoints[NB_CHECKPOINTS];
    mesCheckpoints[0].rect.x = 4096; 
    mesCheckpoints[0].rect.y = 896;
    mesCheckpoints[0].rect.w = 64;   
    mesCheckpoints[0].rect.h = 64;
    mesCheckpoints[0].actif = 0;   
    
    mesCheckpoints[1].rect.x = 10176; 
    mesCheckpoints[1].rect.y = 928;
    mesCheckpoints[1].rect.w = 64;   
    mesCheckpoints[1].rect.h = 64;
    mesCheckpoints[1].actif = 0;   

    Score score;
    init_score(&score, 10, 10);

    Camera camera;
    camera.w = logicalW;
    camera.h = logicalH;
    camera.x = 0;
    camera.y = 0;

    int mapPixelWidth = MAP_WIDTH * TILE_SIZE * MAP_SCALE;
    int mapPixelHeight = MAP_HEIGHT * TILE_SIZE * MAP_SCALE;

    // --- Boucle Principale ---
    int running = 1;
    SDL_Event event;

    while (running) {
        Uint32 frameStart = SDL_GetTicks(); 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) 
                running = 0;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        // -- Mises à jour --
        update_player(&player, keys, tileMap);
        update_loupas(&mechant, tileMap); 
        for (int i = 0; i < NB_THWOMPS; i++) {
            update_thwomp(&mesThwomps[i], &player, tileMap);
        }
        update_score(&score, (int)player.rect.x);
        update_camera(&camera, &player, mapPixelWidth, mapPixelHeight);
        
        if (verifier_conditions_mort(&player, mapPixelHeight)) {
            gerer_mort_joueur(&player, 20, 1000, &score);
            reset_level(&player, &mechant, mesThwomps, mesPodoboo, mesPieces, &score, &camera, 0);
        }
        
        // --- Sauvegarde des scores en temps réel ---
        static int meilleur_score = 0;
        static int score_affichage_fin = 0;

        if (score.value > meilleur_score) { meilleur_score = score.value; }
        if (player.lives > 0) { score_affichage_fin = score.value; }

        // -- Gestion Collision Joueur/Loup --
        if (player.state != STATE_DEAD && mechant.vivant && SDL_HasIntersection(&player.rect, &mechant.rect)) {
            int pieds_avant = (player.rect.y + player.rect.h) - (int)player.velY;
            if (player.velY > 0 && pieds_avant <= mechant.rect.y + 15) {
                mechant.vivant = 0;   
                player.velY = -12.0f; 
            } else {
                player.state = STATE_DEAD; 
                player.velY = -10.0f;      
            }
        }

        // -- Gestion Collision Joueur/Thwomp --
        for (int i = 0; i < NB_THWOMPS; i++) {
            if (player.state != STATE_DEAD && mesThwomps[i].vivant && SDL_HasIntersection(&player.rect, &mesThwomps[i].rect)) {
                player.state = STATE_DEAD; 
                player.velY = -10.0f;
                break; 
            }
        }

        // -- Gestion Collision Joueur/Podoboo --
        for (int i = 0; i < NB_PODOBOO; i++) {
            update_podoboo(&mesPodoboo[i]);

            if (SDL_HasIntersection(&player.rect, &mesPodoboo[i].rect)) {
                if (player.state != STATE_DEAD) {
                    player.state = STATE_DEAD;
                    player.velY = -8.0f; 
                    printf("Brûlé par la lave !\n");
                }
            }
        }

        // -- Gestion Collision Joueur/Pièce --
        for (int i = 0; i < NB_PIECES; i++) {
            if (mesPieces[i].vivant) {
                SDL_Rect rPiece = { mesPieces[i].rect.x, mesPieces[i].rect.y, 32, 32 };
                if (SDL_HasIntersection(&player.rect, &rPiece)) {
                    score.bonus += 50;
                    mesPieces[i].vivant = 0; 
                }
            }
        }

        // -- Gestion des Checkpoint --
        for (int i = 0; i < NB_CHECKPOINTS; i++) {
            if (mesCheckpoints[i].actif == 0) {
                if (SDL_HasIntersection(&player.rect, &mesCheckpoints[i].rect)) {
                    mesCheckpoints[i].actif = 1;
                    mesCheckpoints[i].animStart = SDL_GetTicks(); 
                    player.checkpointX = mesCheckpoints[i].rect.x; 
                    player.checkpointY = mesCheckpoints[i].rect.y; 
                    
                    printf("Checkpoint %d atteint !\n", i);
                }
            }
        }

        // -- Rendu --
        SDL_SetRenderDrawColor(renderer, 27, 45, 45, 255);
        SDL_RenderClear(renderer);

        // Parallax
        draw_parallax_bg(renderer, bg1, camera.x, camera.y, 0.05f, 0.0f, logicalW, logicalH, 0, 0); 
        draw_parallax_bg(renderer, bg4, camera.x, camera.y, 0.08f, 0.02f, logicalW, logicalH, 0, 50);
        draw_parallax_bg(renderer, bg5, camera.x, camera.y, 0.10f, 0.03f, logicalW, logicalH, 0, 150);
        draw_parallax_bg(renderer, bg2, camera.x, camera.y, 0.15f, 0.05f, logicalW, logicalH, 0, 200); 
        draw_parallax_bg(renderer, bg3, camera.x, camera.y, 0.40f, 0.1f, logicalW, logicalH, 0, 350);

        // Background Donjon
        int startDonjonX = 128 * 32; // Début (Tuile 128)
        int endDonjonX = 318 * 32;   // Fin (Tuile 318)

        int xStartEcran = startDonjonX - camera.x;
        int xEndEcran = endDonjonX - camera.x;

        // On définit le clip pour qu'il s'arrête à xEndEcran
        SDL_Rect clip;
        clip.x = xStartEcran;
        clip.y = 0;
        clip.w = xEndEcran - xStartEcran; // La largeur est la distance entre le début et la fin
        clip.h = logicalH;

        SDL_RenderSetClipRect(renderer, &clip);
        draw_parallax_bg(renderer, bg6, camera.x, camera.y, 0.15f, 0.0f, logicalW, logicalH, startDonjonX, 0);
        SDL_RenderSetClipRect(renderer, NULL);
 
        // Background désert
        int startDesertX = 318 * 32; // Début (Tuile 318)
        int mapTotalWidth = MAP_WIDTH * 32; // Fin de la map en pixels
        int xStartEcran2 = startDesertX - camera.x;

        int imgW, imgH;
        SDL_QueryTexture(bg7, NULL, NULL, &imgW, &imgH);
        int biomeWidthInPixels = mapTotalWidth - startDesertX;

        SDL_Rect destDesert = { xStartEcran2, 0, biomeWidthInPixels, logicalH };
        SDL_Rect clipRect;
        clipRect.x = (xStartEcran2 > 0) ? xStartEcran2 : 0;
        clipRect.y = 0;
        clipRect.w = logicalW - clipRect.x;
        clipRect.h = logicalH;

        
        SDL_RenderSetClipRect(renderer, &clipRect);
        SDL_RenderCopy(renderer, bg7, NULL, &destDesert);
        SDL_RenderSetClipRect(renderer, NULL);
        


        // La Pyramide 
        SDL_QueryTexture(texPyramide, NULL, NULL, &imgW, &imgH);
        SDL_SetTextureBlendMode(texPyramide, SDL_BLENDMODE_BLEND); // Effet d'opacité 
        SDL_SetTextureAlphaMod(texPyramide, 76); // 30% d'opacité 

        int finalH = 300; 
        float ratio = (float)finalH / (float)imgH;
        int finalW = (int)(imgW * ratio);
        SDL_Rect posPyramide = { (330 * 32) - camera.x, logicalH - finalH - 250, finalW, finalH };
        SDL_RenderCopy(renderer, texPyramide, NULL, &posPyramide);

        // La Statue 
        int sW, sH;
        SDL_QueryTexture(texStatue, NULL, NULL, &sW, &sH);
        SDL_SetTextureBlendMode(texStatue, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(texStatue, 65);

        int finalStatueH = 250;
        float ratioStatue = (float)finalStatueH / (float)sH;
        int finalStatueW = (int)(sW * ratioStatue);
        SDL_Rect posStatue = { (405 * 32) - camera.x, logicalH-finalStatueH-250, finalStatueW, finalStatueH }; // x, y, w, h 
        SDL_RenderCopy(renderer, texStatue, NULL, &posStatue);

        // Le Château 
        int finNiveauX = (MAP_WIDTH - 15) * 32; 
        SDL_Rect posChateau = { finNiveauX - camera.x, 115, 450, 450 };
        SDL_RenderCopy(renderer, texChateau, NULL, &posChateau);

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
        // On affiche les objets de l'arrière plan vers le premier plan
        for (int i = 0; i < NB_THWOMPS; i++) {
            render_thwomp(renderer, &mesThwomps[i], camera.x, camera.y, texThwomp);
        }

        for (int i = 0; i < NB_PODOBOO; i++) {
            render_podoboo(renderer, &mesPodoboo[i], camera.x, camera.y, texPodoboo);
        }

        render_loupas(renderer, &mechant, camera.x, camera.y, texLoup);
        render_player(renderer, &player, camera.x, camera.y, texIdle, texRun, texJump, texDead);

        for (int i = 0; i < NB_CHECKPOINTS; i++) {
            render_checkpoint(renderer, texCheckpoint, &mesCheckpoints[i], camera.x, camera.y);
        }

        for (int i = 0; i < NB_PIECES; i++) {
            if (mesPieces[i].vivant) {
                render_coin(renderer, texCoin, mesPieces[i].rect.x, mesPieces[i].rect.y, camera.x, camera.y);
            }
        }

        // MODE DEBUG : AFFICHAGE DES HITBOXES 
      
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // 1. Joueur
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128); 
        SDL_Rect debugPlayerRect = { player.rect.x - camera.x, player.rect.y - camera.y, player.rect.w, player.rect.h };
        SDL_RenderFillRect(renderer, &debugPlayerRect); 
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &debugPlayerRect); 

        // 2. Loup
        if (mechant.vivant) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128); 
            SDL_Rect debugEnnemiRect = { mechant.rect.x - camera.x, mechant.rect.y - camera.y, mechant.rect.w, mechant.rect.h };
            SDL_RenderFillRect(renderer, &debugEnnemiRect); 
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &debugEnnemiRect);
        }

        // 3. Thwomp
       for (int i = 0; i < NB_THWOMPS; i++) {
            if (mesThwomps[i].vivant) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 128); // Hitbox bleue
                SDL_Rect debugThwompRect = { mesThwomps[i].rect.x - camera.x, mesThwomps[i].rect.y - camera.y, mesThwomps[i].rect.w, mesThwomps[i].rect.h };
                SDL_RenderFillRect(renderer, &debugThwompRect); 
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawRect(renderer, &debugThwompRect);
            }
        }

        // 4. Podoboo
        for (int i = 0; i < NB_PODOBOO; i++) {
            if (mesPodoboo[i].vivant) {
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, 128); // Hitbox orange
                SDL_Rect debugPodobooRect = { mesPodoboo[i].rect.x - camera.x, mesPodoboo[i].rect.y - camera.y, mesPodoboo[i].rect.w, mesPodoboo[i].rect.h };
                SDL_RenderFillRect(renderer, &debugPodobooRect); 
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                SDL_RenderDrawRect(renderer, &debugPodobooRect);
            }
        }

        // Interface
        render_score(renderer, &score);
        render_lives(renderer, texFullHeart, player.lives);
        render_progress_bar(renderer, player.rect.x, mapPixelWidth);

        // Effet luminosité
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite); 
        SDL_Rect fullScreen = {0, 0, physicalW, physicalH};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_RenderPresent(renderer);

        // --- GESTION DU GAME OVER ---
       if (player.lives <= 0) {
            int action = gameover(renderer, font, &player, score_affichage_fin, meilleur_score);
            if (action == 1) {
                // Paramètre '1' à la fin car on veut tout remettre à zéro (vies + score)
                reset_level(&player, &mechant, mesThwomps, mesPodoboo, mesPieces, &score, &camera, 1);
            } else {
                running = 0;
            }
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) { 
            SDL_Delay(16 - frameTime);
        }
    }
    
    // --- 7. Nettoyage ---
    free(tileMap);
    SDL_DestroyTexture(bg1); SDL_DestroyTexture(bg2); SDL_DestroyTexture(bg3); 
    SDL_DestroyTexture(bg4); SDL_DestroyTexture(bg5); SDL_DestroyTexture(bg6); SDL_DestroyTexture(terrainTex); 
    SDL_DestroyTexture(texIdle); SDL_DestroyTexture(texRun); SDL_DestroyTexture(texJump); SDL_DestroyTexture(texDead);
    SDL_DestroyTexture(texLoup); SDL_DestroyTexture(texThwomp); SDL_DestroyTexture(texPodoboo);
    SDL_DestroyTexture(texCoin); SDL_DestroyTexture(texCheckpoint);
    if (font) TTF_CloseFont(font); 
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();

    return 0;
}