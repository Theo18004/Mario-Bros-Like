/**
 * @file main.c
 * @brief Point d'entrée principal du programme.
 * @details Initialise SDL2, charge les ressources, gère la boucle de jeu.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusion de nos headers persos
#include "map.h"
#include "player.h"
#include "camera.h"
#include "defs.h"
#include "menu.h"
#include "mort.h"
#include "interface.h"
#include "ennemi.h"
#include "Items.h"
#include "flag.h"
#include "level.h"

// --- Pointeurs Audio ---
Mix_Chunk * sonSaut = NULL;
Mix_Chunk * bouleFeu = NULL;
Mix_Chunk * coin = NULL;
Mix_Chunk * sonThwomp = NULL;
Mix_Chunk * sonJC = NULL;
Mix_Music * musiqueSurface = NULL;
Mix_Music * musiqueDonjon = NULL;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // --- 1. Initialisation Globale ---
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return 1;
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (TTF_Init() == -1) return 1;

    TTF_Font* font = TTF_OpenFont("assets/NSuperMario.ttf", 32);
    if (font == NULL) {
        printf("Erreur : impossible de charger la police ! %s\n", TTF_GetError());
    }

    // --- 2. Initialisation Audio ---
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("Erreur Mixer : %s\n", Mix_GetError());
    }
    Mix_AllocateChannels(32);
    Mix_ReserveChannels(1);

    SDL_Window* window = SDL_CreateWindow("Mario-Bros-Like",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int physicalW, physicalH;
    SDL_GetRendererOutputSize(renderer, &physicalW, &physicalH);

    // Charger les paramètres sauvegardés
    charger_parametres();

    // --- 3. CHARGEMENT DE TOUS LES ASSETS GLOBAUX (Une seule fois) ----
    
    // Textures Joueur
    SDL_Texture* texIdle = IMG_LoadTexture(renderer, "assets/Personnage/Idle.png");
    SDL_Texture* texRun  = IMG_LoadTexture(renderer, "assets/Personnage/Run.png");
    SDL_Texture* texJump = IMG_LoadTexture(renderer, "assets/Personnage/Jump.png");
    SDL_Texture* texDead = IMG_LoadTexture(renderer, "assets/Personnage/Dead.png");

    // Textures Ennemis
    SDL_Texture* texLoup = IMG_LoadTexture(renderer, "assets/Ennemi/wolfsheet.png");
    SDL_Texture* texThwomp = IMG_LoadTexture(renderer, "assets/Ennemi/thwompEdit.png");
    SDL_Texture* texPodoboo = IMG_LoadTexture(renderer, "assets/Ennemi/podoboo.png");
    SDL_Texture* texCoquilas = IMG_LoadTexture(renderer, "assets/Ennemi/coquilas_noir.png");
    SDL_Texture* texJeanClaude = IMG_LoadTexture(renderer, "assets/Ennemi/jean-claude.png");
    SDL_Texture* texOlaf = IMG_LoadTexture(renderer, "assets/Ennemi/olaf.png");

    // Textures Items & HUD
    SDL_Texture* texVies[5];
    for (int i=0; i<5; i++) {
        char path[64]; sprintf(path, "assets/Interface/vie_x%d.png", i+1);
        texVies[i] = IMG_LoadTexture(renderer, path);
    }
    SDL_Texture* texCoin = IMG_LoadTexture(renderer, "assets/Items/coin50.png");
    SDL_Texture* texCheckpoint = IMG_LoadTexture(renderer, "assets/Items/Checkpoint.png");
    SDL_Texture* texMat = IMG_LoadTexture(renderer, "assets/Items/Mat.png");
    SDL_Texture* texFlag = IMG_LoadTexture(renderer, "assets/Items/Drapeau.png");

    SDL_Texture* texPiecesHUD[16];
    for (int i = 0; i < 16; i++) {
        char comb[10] = "";
        if (i & 1) { strcat(comb, "1"); } 
        if (i & 2) { strcat(comb, "2"); }
        if (i & 4) { strcat(comb, "3"); } 
        if (i & 8) { strcat(comb, "4"); }
        if (i == 0) strcpy(comb, "0");
        char chemin[128]; sprintf(chemin, "assets/Items/piece_hud/piece_%s.png", comb);
        texPiecesHUD[i] = IMG_LoadTexture(renderer, chemin);
    }

    // Audio - Bruitages
    sonSaut = Mix_LoadWAV("assets/son/Jump.wav");
    bouleFeu = Mix_LoadWAV("assets/son/BouleFeu.wav");
    coin = Mix_LoadWAV("assets/son/coin.wav");
    sonThwomp = Mix_LoadWAV("assets/son/sonThowp.wav");
    sonJC = Mix_LoadWAV("assets/son/sonJC.wav");
    
    // Audio - Musiques 
    musiqueSurface = Mix_LoadMUS("assets/son/Above_the_Floating_Isles.wav");
    musiqueDonjon = Mix_LoadMUS("assets/son/Beneath_The_Masonry.wav");

    // Appliquer le volume (bruitages ET musiques)
    Mix_Volume(-1, (volume * 128) / 100);
    Mix_VolumeMusic((volume * 128) / 100);

    // Menu Pause - Boutons (La création des rects est gérée dans interface.c)
    SDL_Texture* texIcons = IMG_LoadTexture(renderer, "assets/bouton.png");

    // Variables persistantes entre les parties
    static int meilleur_score = 0;
    static int score_affichage_fin = 0;

    int application_ouverte = 1;

    // --- BOUCLE D'APPLICATION (Permet de retourner au menu principal) ---
    while (application_ouverte) {
        
        // 1. On remet les coordonnées en mode "Ecran physique" pour le menu
        SDL_RenderSetLogicalSize(renderer, 0, 0);

        // 2. Affichage du Menu Principal
        MenuResult menuChoix = afficher_menu(renderer, physicalW, physicalH);
        if (menuChoix == MENU_QUIT) {
            application_ouverte = 0; 
            break;
        }

        // 3. Configuration de la résolution "Jeu" (Pixels Art)
        float aspectRatio = (float)physicalW / (float)physicalH;
        int logicalH = REFERENCE_HEIGHT;
        int logicalW = (int)(REFERENCE_HEIGHT * aspectRatio);
        SDL_RenderSetLogicalSize(renderer, logicalW, logicalH);

        // Initialisation propre de la structure du menu pause (qui est dans interface.h)
        PauseMenu menuPause;
        init_pause_menu(&menuPause, texIcons, logicalW, logicalH);

        // 4. Chargement du niveau sélectionné
        int idMap = (menuChoix == MENU_CHOIX_MAP2) ? 2 : 1;
        Level* lvl = load_level(renderer, idMap); 
        if (!lvl) {
            printf("Erreur : Impossible de charger le niveau %d\n", idMap);
            application_ouverte = 0;
            break;
        }

        // --- LANCEMENT DE LA MUSIQUE ---
        Mix_HaltMusic(); 
        if (idMap == 1 && musiqueSurface) {
            Mix_PlayMusic(musiqueSurface, -1); 
        } else if (idMap == 2 && musiqueDonjon) {
            Mix_PlayMusic(musiqueDonjon, -1);
        }

        // 5. Initialisation des objets du niveau
        Player player;
        init_player(&player, lvl->playerStart.x, lvl->playerStart.y);
        player.lives = 3;

        Ennemi mesLoupas[NB_LOUPAS];
        Thwomp mesThwomps[NB_THWOMPS];
        Podoboo mesPodoboo[NB_PODOBOO];
        Coquilas mesCoquilas[NB_COQUILAS];
        Ennemi jc[NB_JEAN_CLAUDE];
        Ennemi mesOlaf[NB_OLAF];
        Piece mesPieces[NB_PIECES];
        Checkpoint mesCheckpoints[NB_CHECKPOINTS];
        Flag monDrapeau;

        // Spawn des entités selon le niveau (incluant Olaf)
        spawn_level_entities(lvl, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, mesOlaf, mesPieces, mesCheckpoints, &monDrapeau);

        Score score;
        init_score(&score);

        Camera camera;
        camera.w = logicalW; camera.h = logicalH;
        camera.x = 0; camera.y = 0;

        int mapPixelWidth = lvl->mapPixelWidth;
        int mapPixelHeight = lvl->mapPixelHeight;

        int running = 1; 
        int enPause = 0; 
        SDL_Event event;
        int tempsMax = 250;
        Uint32 startTime = SDL_GetTicks();
        int tempsRestant = tempsMax;

        // --- BOUCLE DE JEU (Niveau Actuel) ---
        while (running) {
            Uint32 frameStart = SDL_GetTicks();
            
            // --- GESTION DES EVENEMENTS ---
            while (SDL_PollEvent(&event)) {
                // 1. Quitter le jeu
                if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    running = 0; 
                    application_ouverte = 0; // Ferme tout
                }

                // 2. Touche 'P' pour la Pause
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                    enPause = !enPause; 
                    if (enPause) Mix_PauseMusic();
                    else Mix_ResumeMusic();
                }

                // 3. Gestion Souris (clic sur les boutons)
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_Point p = {event.button.x, event.button.y};

                    if (!enPause) {
                        if (SDL_PointInRect(&p, &menuPause.dstPausePos)) {
                            enPause = 1;
                            Mix_PauseMusic();
                        }
                    } else {
                        if (SDL_PointInRect(&p, &menuPause.dstPlay)) {
                            enPause = 0; 
                            Mix_ResumeMusic();
                        }
                        if (SDL_PointInRect(&p, &menuPause.dstRestart)) {         
                            enPause = 0;
                            
                            Mix_HaltMusic(); 
                            if (idMap == 1 && musiqueSurface) Mix_PlayMusic(musiqueSurface, -1);
                            else if (idMap == 2 && musiqueDonjon) Mix_PlayMusic(musiqueDonjon, -1);
                            reset_level(&player, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, mesOlaf, mesPieces, &score, &camera, 1, lvl->id);
                            for (int i = 0; i < NB_CHECKPOINTS; i++) mesCheckpoints[i].actif = 0;
                            startTime = SDL_GetTicks();
                            tempsRestant = tempsMax;
                        }
                        if (SDL_PointInRect(&p, &menuPause.dstHome)) { 
                            Mix_HaltMusic(); 
                            running = 0; 
                        }
                    }
                }
            }

            const Uint8* keys = SDL_GetKeyboardState(NULL);

            if (!enPause) {
                // 1. Update Entities (Avec lvl->id)
                update_player(&player, keys, lvl->tileMap, lvl->id);
                for (int i = 0; i < NB_LOUPAS; i++) update_loupas(&mesLoupas[i], lvl->tileMap, lvl->id);
                for (int i = 0; i < NB_THWOMPS; i++) update_thwomp(&mesThwomps[i], &player, lvl->tileMap, lvl->id);
                for (int i = 0; i < NB_COQUILAS; i++) update_coquilas(&mesCoquilas[i], lvl->tileMap, lvl->id);
                for (int i = 0; i < NB_JEAN_CLAUDE; i++) update_jc(&jc[i], lvl->tileMap, lvl->id);
                for (int i = 0; i < NB_OLAF; i++) update_snowman(&mesOlaf[i], lvl->tileMap, lvl->id);

                update_score(&score, (int)player.rect.x);
                update_camera(&camera, &player, mapPixelWidth, mapPixelHeight);

                // 2. Gestion Mort
                if (verifier_conditions_mort(&player, lvl->mapPixelHeight)) {
                    int temps_sauvegarde = tempsRestant;
                    if (temps_sauvegarde <= 0) temps_sauvegarde = tempsMax; 

                    gerer_mort_joueur(&player, lvl->playerStart.x, lvl->playerStart.y, &score);
                    reset_level(&player, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, mesOlaf, mesPieces, &score, &camera, 0, lvl->id);
                    startTime = SDL_GetTicks() - ((tempsMax - temps_sauvegarde) * 1000);
                    tempsRestant = temps_sauvegarde;
                }

                // 3. Chrono & Scores
                if (player.state != STATE_WIN) {
                    int tempsEcoule = (SDL_GetTicks() - startTime) / 1000;
                    tempsRestant = tempsMax - tempsEcoule;
                }

                if (tempsRestant <= 0) {
                    tempsRestant = 0;
                    if (player.state != STATE_DEAD) {
                        player.state = STATE_DEAD;
                        player.velY = -10.0f;
                    }
                }

                if (score.value > meilleur_score) { meilleur_score = score.value; }
                if (player.lives > 0) { score_affichage_fin = score.value; }

                // 4. Collisions
                // -- Loup --
                for (int i = 0; i < NB_LOUPAS; i++) {
                    if (player.state != STATE_DEAD && mesLoupas[i].vivant && SDL_HasIntersection(&player.rect, &mesLoupas[i].rect)) {
                        int pieds_avant = (player.rect.y + player.rect.h) - (int)player.velY;
                        if (player.velY > 0 && pieds_avant <= mesLoupas[i].rect.y + 15) {
                            mesLoupas[i].vivant = 0; score.bonus += 100; player.velY = -12.0f;
                        } else { player.state = STATE_DEAD; player.velY = -10.0f; }
                    }
                }

                // -- Thwomp --
                for (int i = 0; i < NB_THWOMPS; i++) {
                    if (player.state != STATE_DEAD && mesThwomps[i].vivant && SDL_HasIntersection(&player.rect, &mesThwomps[i].rect)) {
                        player.state = STATE_DEAD; player.velY = -10.0f; break;
                    }
                }

                // -- Podoboo --
                for (int i = 0; i < NB_PODOBOO; i++) {
                    update_podoboo(&mesPodoboo[i], &camera);
                    if (SDL_HasIntersection(&player.rect, &mesPodoboo[i].rect)) {
                        if (player.state != STATE_DEAD) { player.state = STATE_DEAD; player.velY = -8.0f; }
                    }
                }

                // -- Coquilas --
                for (int i = 0; i < NB_COQUILAS; i++) {
                    if (player.state != STATE_DEAD && mesCoquilas[i].vivant && SDL_HasIntersection(&player.rect, &mesCoquilas[i].rect)) {
                        player.state = STATE_DEAD; player.velY = -10.0f;
                    }
                }

                // -- Jean-Claude --
                for (int i = 0; i < NB_JEAN_CLAUDE; i++) {
                    if (player.state != STATE_DEAD && jc[i].vivant && SDL_HasIntersection(&player.rect, &jc[i].rect)) {
                        if (player.velY > 0 && (player.rect.y + player.rect.h) < (jc[i].rect.y + 30)) {
                            jc[i].vivant = 0; player.velY = -12.0f;
                            if(sonJC != NULL) Mix_PlayChannel(-1, sonJC, 0);
                        } else { player.state = STATE_DEAD; player.velY = -10.0f; }
                    }
                }

                // -- Olaf --
                for (int i = 0; i < NB_OLAF; i++) {
                    if (player.state != STATE_DEAD && mesOlaf[i].vivant && SDL_HasIntersection(&player.rect, &mesOlaf[i].rect)) {
                        if (player.velY > 0 && (player.rect.y + player.rect.h) < (mesOlaf[i].rect.y + 20)) {
                            mesOlaf[i].vivant = 0; player.velY = -12.0f; score.bonus += 150;
                        } else { player.state = STATE_DEAD; player.velY = -10.0f; }
                    }
                }

                // -- Pièces --
                for (int i = 0; i < NB_PIECES; i++) {
                    if (mesPieces[i].vivant) {
                        SDL_Rect rPiece = { mesPieces[i].rect.x, mesPieces[i].rect.y, 32, 32 };
                        if (player.state != STATE_DEAD && SDL_HasIntersection(&player.rect, &rPiece)) {
                            score.bonus += 50; mesPieces[i].vivant = 0;
                            if(coin != NULL) Mix_PlayChannel(-1, coin, 0);
                        }
                    }
                }

                // -- Checkpoints --
                for (int i = 0; i < NB_CHECKPOINTS; i++) {
                    if (mesCheckpoints[i].actif == 0 && SDL_HasIntersection(&player.rect, &mesCheckpoints[i].rect)) {
                        mesCheckpoints[i].actif = 1; mesCheckpoints[i].animStart = SDL_GetTicks();
                        player.checkpointX = mesCheckpoints[i].rect.x; player.checkpointY = mesCheckpoints[i].rect.y;
                        printf("Checkpoint %d atteint !\n", i);
                    }
                }

                // 5. Victoire (Drapeau)
                if (monDrapeau.actif == 0 && SDL_HasIntersection(&player.rect, &monDrapeau.matRect)) {
                    monDrapeau.actif = 1; player.state = STATE_WIN;
                    player.rect.x = monDrapeau.matRect.x - player.rect.w / 2;
                }
                update_flag(&monDrapeau);

                if (monDrapeau.actif == 2 && player.state == STATE_WIN) {
                    Mix_HaltMusic(); 
                    SDL_Delay(500);
                    int etoiles = 0;
                    for (int i = 0; i < NB_PIECES; i++) { if (mesPieces[i].vivant == 0) etoiles++; }
                    
                    int action = victory_screen(renderer, font, &player, &score, tempsRestant, etoiles);
                    (void)action;
                    running = 0; 
                }

            } else {
                startTime += (SDL_GetTicks() - frameStart); // Geler le chrono si en pause
            }

            // --- RENDU VISUEL ---
            SDL_SetRenderDrawColor(renderer, 27, 45, 45, 255);
            SDL_RenderClear(renderer);

            draw_level_backgrounds(renderer, lvl, camera.x, camera.y, logicalW, logicalH);

            for (int y = 0; y < MAP_HEIGHT; y++) {
                for (int x = 0; x < MAP_WIDTH; x++) {
                    int tilePixelX = x * TILE_SIZE * MAP_SCALE;
                    if (tilePixelX > camera.x - 64 && tilePixelX < camera.x + logicalW + 64) {
                        draw_tile(renderer, &lvl->tileset, lvl->tileMap[y * MAP_WIDTH + x], tilePixelX, y * TILE_SIZE * MAP_SCALE, camera.x, camera.y);
                    }
                }
            }

            for(int i=0; i<NB_LOUPAS; i++) render_loupas(renderer, &mesLoupas[i], camera.x, camera.y, texLoup);
            for(int i=0; i<NB_THWOMPS; i++) render_thwomp(renderer, &mesThwomps[i], camera.x, camera.y, texThwomp);
            for(int i=0; i<NB_PODOBOO; i++) render_podoboo(renderer, &mesPodoboo[i], camera.x, camera.y, texPodoboo);
            for(int i=0; i<NB_COQUILAS; i++) render_coquilas(renderer, &mesCoquilas[i], camera.x, camera.y, texCoquilas);
            for(int i=0; i<NB_JEAN_CLAUDE; i++) render_jc(renderer, &jc[i], camera.x, camera.y, texJeanClaude);
            for(int i=0; i<NB_CHECKPOINTS; i++) render_checkpoint(renderer, texCheckpoint, &mesCheckpoints[i], camera.x, camera.y);
            for(int i=0; i<NB_PIECES; i++) if (mesPieces[i].vivant) render_coin(renderer, texCoin, mesPieces[i].rect.x, mesPieces[i].rect.y, camera.x, camera.y);
            for(int i=0; i<NB_OLAF; i++) render_snowman(renderer, &mesOlaf[i], camera.x, camera.y, texOlaf);

            if (hitboxes) render_debug_hitboxes(renderer, &player, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, &monDrapeau, camera.x, camera.y);

            render_score(renderer, font, &score);
            render_lives(renderer, texVies, player.lives);
            render_pieces_hud(renderer, texPiecesHUD, mesPieces);
            render_timer(renderer, font, tempsRestant);
            render_flag(renderer, &monDrapeau, texMat, texFlag, camera.x, camera.y);
            render_player(renderer, &player, camera.x, camera.y, texIdle, texRun, texJump, texDead);

            // Luminosité gérée par les paramètres
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite);
            SDL_Rect fs = {0, 0, logicalW, logicalH};
            SDL_RenderFillRect(renderer, &fs);

            // Menu Pause centralisé 
            render_pause_menu(renderer, &menuPause, enPause, logicalW, logicalH);

            SDL_RenderPresent(renderer);

            // Gestion Game Over
            if (player.lives <= 0) {
                Mix_HaltMusic(); // Arrête la musique sur l'écran Game Over
                int action = gameover(renderer, font, &player, score_affichage_fin, meilleur_score);
                if (action == 1) {
                    reset_level(&player, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, mesOlaf, mesPieces, &score, &camera, 1, lvl->id);
                    for (int i = 0; i < NB_CHECKPOINTS; i++) mesCheckpoints[i].actif = 0;
                    startTime = SDL_GetTicks(); tempsRestant = tempsMax;
                    
                    // Relance la musique après un "Try Again"
                    if (idMap == 1 && musiqueSurface) Mix_PlayMusic(musiqueSurface, -1);
                    else if (idMap == 2 && musiqueDonjon) Mix_PlayMusic(musiqueDonjon, -1);
                } else {
                    running = 0; // Retour au menu !
                }
            }

            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < 16) SDL_Delay(16 - frameTime);
        }

        // --- NETTOYAGE DU NIVEAU (Fin de la boucle de jeu, avant de retourner au menu) ---
        cleanup_level(lvl);
    }

    // --- 7. NETTOYAGE GLOBAL (Fermeture du jeu) ---
    for (int i = 0; i < 5; i++) SDL_DestroyTexture(texVies[i]);
    for (int i = 0; i < 16; i++) if (texPiecesHUD[i]) SDL_DestroyTexture(texPiecesHUD[i]);
    
    // Libération Audio
    Mix_FreeChunk(sonSaut); Mix_FreeChunk(bouleFeu); Mix_FreeChunk(coin); 
    Mix_FreeChunk(sonThwomp); Mix_FreeChunk(sonJC);
    if (musiqueSurface) Mix_FreeMusic(musiqueSurface);
    if (musiqueDonjon) Mix_FreeMusic(musiqueDonjon);
    Mix_CloseAudio();
    
    // Libération Graphique
    SDL_DestroyTexture(texIdle); SDL_DestroyTexture(texRun); SDL_DestroyTexture(texJump); SDL_DestroyTexture(texDead);
    SDL_DestroyTexture(texLoup); SDL_DestroyTexture(texThwomp); SDL_DestroyTexture(texPodoboo); SDL_DestroyTexture(texCoquilas); SDL_DestroyTexture(texJeanClaude); SDL_DestroyTexture(texOlaf);
    SDL_DestroyTexture(texCoin); SDL_DestroyTexture(texCheckpoint); SDL_DestroyTexture(texMat); SDL_DestroyTexture(texFlag);
    if (texIcons) SDL_DestroyTexture(texIcons); 
    if (font) TTF_CloseFont(font);
    
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();

    return 0;
}