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

/**
 * @brief Fonction main.
 */

Mix_Chunk * sonSaut = NULL;
Mix_Chunk * bouleFeu = NULL;
Mix_Chunk * coin = NULL;
Mix_Chunk * sonThwomp = NULL;
Mix_Chunk * sonJC = NULL;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // --- 1. Initialisation Globale ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    IMG_Init(IMG_INIT_PNG);
    if (TTF_Init() == -1) return 1;

    TTF_Font* font = TTF_OpenFont("assets/NSuperMario.ttf", 32);
    if (font == NULL) {
        printf("Erreur : impossible de charger la police ! %s\n", TTF_GetError());
    }

    // --- 2. Initialisation Audio ---
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0){
        printf("Erreur Mixer : %s\n", Mix_GetError());
    }

    Mix_AllocateChannels(32);
    Mix_ReserveChannels(1);

    SDL_Window* window = SDL_CreateWindow("Mario-Bros-Like",
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

    // Chargement du niveau (Map 1 par défaut)
    int idMap = 1;
    if (menuChoix == MENU_CHOIX_MAP2) idMap = 2;

    Level* lvl = load_level(renderer, idMap); 
    if (!lvl) {
        printf("Erreur : Impossible de charger le niveau %d\n", idMap);
        return 1;
    }

    // --- 4. Chargement Assets Globaux ---
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

    // Audio
    sonSaut = Mix_LoadWAV("assets/son/Jump.wav");
    bouleFeu = Mix_LoadWAV("assets/son/BouleFeu.wav");
    coin = Mix_LoadWAV("assets/son/coin.wav");
    sonThwomp = Mix_LoadWAV("assets/son/sonThowp.wav");
    sonJC = Mix_LoadWAV("assets/son/sonJC.wav");
    Mix_Volume(-1, (volume * 128) / 100);

    // --- 5. Initialisation Objets ---
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

    // Spawn des entités selon le niveau
    spawn_level_entities(lvl, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, mesOlaf, mesPieces, mesCheckpoints, &monDrapeau);

    Score score;
    init_score(&score);

    Camera camera;
    camera.w = logicalW; camera.h = logicalH;
    camera.x = 0; camera.y = 0;

    int mapPixelWidth = lvl->mapPixelWidth;
    int mapPixelHeight = lvl->mapPixelHeight;

    // --- Boucle Principale ---
    int running = 1;
    SDL_Event event;
    int tempsMax = 250;
    Uint32 startTime = SDL_GetTicks();
    int tempsRestant = tempsMax;

    // --- Sauvegarde des scores en temps réel ---
        static int meilleur_score = 0;
        static int score_affichage_fin = 0;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                running = 0;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        // -- Mises à jour --
        update_player(&player, keys, lvl->tileMap, lvl->id);
        for (int i = 0; i < NB_LOUPAS; i++) update_loupas(&mesLoupas[i], lvl->tileMap, lvl->id);
        for (int i = 0; i < NB_THWOMPS; i++) update_thwomp(&mesThwomps[i], &player, lvl->tileMap, lvl->id);
        for (int i = 0; i < NB_COQUILAS; i++) update_coquilas(&mesCoquilas[i], lvl->tileMap, lvl->id);
        for (int i = 0; i < NB_JEAN_CLAUDE; i++) update_jc(&jc[i], lvl->tileMap, lvl->id);
        for (int i = 0; i < NB_OLAF; i++) update_snowman(&mesOlaf[i], lvl->tileMap, lvl->id);

        update_score(&score, (int)player.rect.x);
        update_camera(&camera, &player, mapPixelWidth, mapPixelHeight);

        // --- MORT ET RESPAWN ---
        if (verifier_conditions_mort(&player, lvl->mapPixelHeight)) {
            int temps_sauvegarde = tempsRestant;
            if (temps_sauvegarde <= 0) temps_sauvegarde = tempsMax; 

            gerer_mort_joueur(&player, lvl->playerStart.x, lvl->playerStart.y, &score);
            reset_level(&player, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, mesOlaf, mesPieces, &score, &camera, 0, lvl->id);
            startTime = SDL_GetTicks() - ((tempsMax - temps_sauvegarde) * 1000);
            tempsRestant = temps_sauvegarde;
        }

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

        // -- Gestion Collision Joueur/Loup --
         for (int i = 0; i < NB_LOUPAS; i++) {
            if (player.state != STATE_DEAD && mesLoupas[i].vivant && SDL_HasIntersection(&player.rect, &mesLoupas[i].rect)) {
                int pieds_avant = (player.rect.y + player.rect.h) - (int)player.velY;
                if (player.velY > 0 && pieds_avant <= mesLoupas[i].rect.y + 15) {
                    mesLoupas[i].vivant = 0;
                    score.bonus += 100;
                    player.velY = -12.0f;
                } else {
                    player.state = STATE_DEAD;
                    player.velY = -10.0f;
                }
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
            update_podoboo(&mesPodoboo[i], &camera);

            if (SDL_HasIntersection(&player.rect, &mesPodoboo[i].rect)) {
                if (player.state != STATE_DEAD) {
                    player.state = STATE_DEAD;
                    player.velY = -8.0f;
                }
            }
        }

        // -- Gestion Collision Joueur/Coquilas --
        for (int i = 0; i < NB_COQUILAS; i++) {
            if (player.state != STATE_DEAD && mesCoquilas[i].vivant && SDL_HasIntersection(&player.rect, &mesCoquilas[i].rect)) {
                player.state = STATE_DEAD;
                player.velY = -10.0f;
            }
        }

        // -- Gestion Collision Joueur/Jean-Claude --
        for (int i = 0; i < NB_JEAN_CLAUDE; i++) {
            if (player.state != STATE_DEAD && jc[i].vivant && SDL_HasIntersection(&player.rect, &jc[i].rect)) {
                if (player.velY > 0 && (player.rect.y + player.rect.h) < (jc[i].rect.y + 30)) {
                    jc[i].vivant = 0;
                    player.velY = -12.0f;
                    if(sonJC != NULL) {
                        Mix_PlayChannel(-1, sonJC, 0);
                    }
                } else {
                    player.state = STATE_DEAD;
                    player.velY = -10.0f;
                }
            }
        }

        // -- Gestion Collision Joueur/Olaf
        for (int i = 0; i < NB_OLAF; i++) {
            if (player.state != STATE_DEAD && mesOlaf[i].vivant && SDL_HasIntersection(&player.rect, &mesOlaf[i].rect)) {
                if (player.velY > 0 && (player.rect.y + player.rect.h) < (mesOlaf[i].rect.y + 20)) {
                    mesOlaf[i].vivant = 0;
                    player.velY = -12.0f;
                    score.bonus += 150;
                } else {
                    player.state = STATE_DEAD;
                    player.velY = -10.0f;
                }
            }
        }

        // -- Gestion Collision Joueur/Pièce --
        for (int i = 0; i < NB_PIECES; i++) {
            if (mesPieces[i].vivant) {
                SDL_Rect rPiece = { mesPieces[i].rect.x, mesPieces[i].rect.y, 32, 32 };
                if (player.state != STATE_DEAD && SDL_HasIntersection(&player.rect, &rPiece)) {
                    score.bonus += 50;
                    mesPieces[i].vivant = 0;

                    if(coin != NULL) {
                        Mix_PlayChannel(-1, coin, 0);
                    }
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

        // Gestion de victoire
        // 1. Activation du drapeau (si le joueur touche le mât )
        if (monDrapeau.actif == 0 && SDL_HasIntersection(&player.rect, &monDrapeau.matRect)) {
            monDrapeau.actif = 1;
            player.state = STATE_WIN;
            player.rect.x = monDrapeau.matRect.x - player.rect.w / 2;

        }
        // 2. Fait descendre le drapeau
        update_flag(&monDrapeau);

        // 3. Conversion du temps en score (Quand le drapeau est au sol)
       if (monDrapeau.actif == 2 && player.state == STATE_WIN) {
            SDL_Delay(500);

            // Calcul des étoiles ramassées
            int etoiles = 0;

            for (int i = 0; i < NB_PIECES; i++) {
                if (mesPieces[i].vivant == 0) etoiles++;
            }

            // On affiche le menu de victoire
            int action = victory_screen(renderer, font, &player, &score, tempsRestant, etoiles);

            if (action == 1) {
                running = 0;
            } else {
                running = 0;
            }
        }

        // --- RENDU ---
        SDL_SetRenderDrawColor(renderer, 27, 45, 45, 255);
        SDL_RenderClear(renderer);

        // 1. Backgrounds du niveau
        draw_level_backgrounds(renderer, lvl, camera.x, camera.y, logicalW, logicalH);

        // 2. Dessin de la Map
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tilePixelX = x * TILE_SIZE * MAP_SCALE;
                if (tilePixelX > camera.x - 64 && tilePixelX < camera.x + logicalW + 64) {
                    draw_tile(renderer, &lvl->tileset, lvl->tileMap[y * MAP_WIDTH + x], tilePixelX, y * TILE_SIZE * MAP_SCALE, camera.x, camera.y);
                }
            }
        }

        // 3. Entités
        for(int i=0; i<NB_LOUPAS; i++) render_loupas(renderer, &mesLoupas[i], camera.x, camera.y, texLoup);
        for(int i=0; i<NB_THWOMPS; i++) render_thwomp(renderer, &mesThwomps[i], camera.x, camera.y, texThwomp);
        for(int i=0; i<NB_PODOBOO; i++) render_podoboo(renderer, &mesPodoboo[i], camera.x, camera.y, texPodoboo);
        for(int i=0; i<NB_COQUILAS; i++) render_coquilas(renderer, &mesCoquilas[i], camera.x, camera.y, texCoquilas);
        for(int i=0; i<NB_JEAN_CLAUDE; i++) render_jc(renderer, &jc[i], camera.x, camera.y, texJeanClaude);
        for(int i=0; i<NB_CHECKPOINTS; i++) render_checkpoint(renderer, texCheckpoint, &mesCheckpoints[i], camera.x, camera.y);
        for(int i=0; i<NB_PIECES; i++) if (mesPieces[i].vivant) render_coin(renderer, texCoin, mesPieces[i].rect.x, mesPieces[i].rect.y, camera.x, camera.y);
        for (int i = 0; i < NB_OLAF; i++) render_snowman(renderer, &mesOlaf[i], camera.x, camera.y, texOlaf);

        if (hitboxes) render_debug_hitboxes(renderer, &player, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, &monDrapeau, camera.x, camera.y);

        // 4. Interface
        render_score(renderer, font, &score);
        render_lives(renderer, texVies, player.lives);
        render_pieces_hud(renderer, texPiecesHUD, mesPieces);
        render_timer(renderer, font, tempsRestant);
        render_flag(renderer, &monDrapeau, texMat, texFlag, camera.x, camera.y);
        render_player(renderer, &player, camera.x, camera.y, texIdle, texRun, texJump, texDead);

        // Effet luminosité
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite);
        SDL_Rect fs = {0, 0, physicalW, physicalH};
        SDL_RenderFillRect(renderer, &fs);

        SDL_RenderPresent(renderer);

        // Game Over
        if (player.lives <= 0) {
            int action = gameover(renderer, font, &player, score_affichage_fin, meilleur_score);
            if (action == 1) {
                reset_level(&player, mesLoupas, mesThwomps, mesPodoboo, mesCoquilas, jc, mesOlaf, mesPieces, &score, &camera, 1, lvl->id);
                for (int i = 0; i < NB_CHECKPOINTS; i++) mesCheckpoints[i].actif = 0;
                startTime = SDL_GetTicks(); tempsRestant = tempsMax;
            } else running = 0;
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) SDL_Delay(16 - frameTime);
    }

    // --- 7. Nettoyage ---
    cleanup_level(lvl);
    for (int i = 0; i < 5; i++) SDL_DestroyTexture(texVies[i]);
    for (int i = 0; i < 16; i++) if (texPiecesHUD[i]) SDL_DestroyTexture(texPiecesHUD[i]);
    Mix_FreeChunk(sonSaut); Mix_FreeChunk(bouleFeu); Mix_FreeChunk(coin); Mix_FreeChunk(sonThwomp); Mix_FreeChunk(sonJC); Mix_CloseAudio();
    SDL_DestroyTexture(texIdle); SDL_DestroyTexture(texRun); SDL_DestroyTexture(texJump); SDL_DestroyTexture(texDead);
    SDL_DestroyTexture(texLoup); SDL_DestroyTexture(texThwomp); SDL_DestroyTexture(texPodoboo); SDL_DestroyTexture(texCoquilas); SDL_DestroyTexture(texJeanClaude); SDL_DestroyTexture(texOlaf);
    SDL_DestroyTexture(texCoin); SDL_DestroyTexture(texCheckpoint); SDL_DestroyTexture(texMat); SDL_DestroyTexture(texFlag);
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();

    return 0;
}