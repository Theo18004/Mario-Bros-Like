#include "level.h"
#include <stdlib.h>
#include <stdio.h>

Level* load_level(SDL_Renderer* renderer, int levelID) {
    Level* lvl = calloc(1, sizeof(Level));
    if (!lvl) return NULL;
    lvl->id = levelID;

    if (levelID == 1) {
        // --- CHARGEMENT MAP 1 ---
        lvl->tileMap = (int*)malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
        load_map_from_csv("assets/Maps/map_v3.csv", lvl->tileMap);
        
        SDL_Texture* tTex = IMG_LoadTexture(renderer, "assets/Terrain/CustomTileset.png");
        lvl->tileset = (Tileset){ tTex, 22, 16, 16 };

        lvl->playerStart = (SDL_Point){ 20, 1000 };

        // Backgrounds 
        lvl->bgs[0] = IMG_LoadTexture(renderer, "assets/Sprites/Background/sky.png");
        lvl->bgs[1] = IMG_LoadTexture(renderer, "assets/Sprites/Background/mountain2.png");
        lvl->bgs[2] = IMG_LoadTexture(renderer, "assets/Sprites/Background/pine1.png");
        lvl->bgs[3] = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud1.png");
        lvl->bgs[4] = IMG_LoadTexture(renderer, "assets/Sprites/Background/cloud2.png");
        lvl->bgs[5] = IMG_LoadTexture(renderer, "assets/Sprites/Background/Donjon.png");
        lvl->bgs[6] = IMG_LoadTexture(renderer, "assets/Sprites/Background/desert2.png");

        // Décors fixes
        lvl->bgs[11] = IMG_LoadTexture(renderer, "assets/Sprites/Background/statue.png");
        lvl->bgsPos[11] = (SDL_Rect){ 405 * 32, 0, 0, 250 }; 

        lvl->bgs[12] = IMG_LoadTexture(renderer, "assets/Sprites/Background/pyramide.png");
        lvl->bgsPos[12] = (SDL_Rect){ 330 * 32, 0, 0, 300 };

        lvl->bgs[13] = IMG_LoadTexture(renderer, "assets/Sprites/Background/chateau.png");
        lvl->bgsPos[13] = (SDL_Rect){ (MAP_WIDTH - 15) * 32, 115, 450, 450 };
    } 

    // --- MAP 2 --- 
    else if(levelID == 2) {
        // --- CHARGEMENT MAP 2 ---
        lvl->tileMap = (int*)malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
        load_map_from_csv("assets/Maps/map2_v0.1.csv", lvl->tileMap);
        SDL_Texture* tTex = IMG_LoadTexture(renderer, "assets/Terrain/CustomTilesetMap2.png");
        lvl->tileset = (Tileset){ tTex, 22, 16, 16 };
        lvl->playerStart = (SDL_Point){ 11648, 700 };

        // Backgrounds 
        lvl->bgs[0] = IMG_LoadTexture(renderer, "assets/Sprites/BackgroundMap2/BG.png");
        lvl->bgs[1] = IMG_LoadTexture(renderer, "assets/Sprites/BackgroundMap2/lune1.png"); // 1er plan crateres + sol
        lvl->bgs[2] = IMG_LoadTexture(renderer, "assets/Sprites/BackgroundMap2/lune2.png"); // 2eme plan carteres seul
        lvl->bgs[3] = IMG_LoadTexture(renderer, "assets/Sprites/BackgroundMap2/lune3.png"); // 3eme plan montagnes
        lvl->bgs[4] = IMG_LoadTexture(renderer, "assets/Sprites/BackgroundMap2/lunebg.png"); // 4eme plan espace
        lvl->bgs[5] = IMG_LoadTexture(renderer, "assets/Sprites/BackgroundMap2/villefull.png"); 
    }

    lvl->mapPixelWidth = MAP_WIDTH * TILE_SIZE * MAP_SCALE;
    lvl->mapPixelHeight = MAP_HEIGHT * TILE_SIZE * MAP_SCALE;
    return lvl;
}

void spawn_level_entities(Level* lvl, Ennemi* loupas, Thwomp* thwomps, Podoboo* podoboo, Coquilas* coquilas, Ennemi* jc, Ennemi* olaf,Presse* presse,Ennemi* mesAliens, Piece* pieces, Checkpoint* checkpoints, Flag* flag) {
    for (int i = 0; i < NB_LOUPAS; i++) loupas[i].vivant = 0;
    for (int i = 0; i < NB_THWOMPS; i++) thwomps[i].vivant = 0;
    for (int i = 0; i < NB_PODOBOO; i++) podoboo[i].vivant = 0;
    for (int i = 0; i < NB_COQUILAS; i++) coquilas[i].vivant = 0;
    for (int i = 0; i < NB_JEAN_CLAUDE; i++) jc[i].vivant = 0;
    for (int i = 0; i < NB_OLAF; i++) olaf[i].vivant = 0;
    for (int i = 0; i < NB_ALIENS; i++) mesAliens[i].vivant = 0;   
    for (int i = 0; i < NB_PRESSES; i++) presse[i].vivant = 0;
    for (int i = 0; i < NB_PIECES; i++) pieces[i].vivant = 1;
    for (int i = 0; i < NB_CHECKPOINTS; i++) checkpoints[i].actif = 0;

    // --- MAP 1 ---
    if (lvl->id == 1) {
        // Coordonnées des ennemis
        // -- Loupas --
        init_loupas(&loupas[0], 600, 1000); 
        init_loupas(&loupas[1], 1184, 768);
        init_loupas(&loupas[2], 1920, 576); 
        init_loupas(&loupas[3], 2368, 576);
        init_loupas(&loupas[4], 3072, 1056);

        // -- Thwomps --
        init_thwomp(&thwomps[0], 8960, 672);
        init_thwomp(&thwomps[1], 9120, 672);
        init_thwomp(&thwomps[2], 9250, 672);  
        init_thwomp(&thwomps[3], 9340, 672);
        init_thwomp(&thwomps[4], 9400, 672); 
        init_thwomp(&thwomps[5], 9568, 672);
        init_thwomp(&thwomps[6], 9728, 672); 
        init_thwomp(&thwomps[7], 9888, 672);

        // -- Podoboos --
        init_podoboo(&podoboo[0], 7296, 1088, 800); 
        init_podoboo(&podoboo[1], 7488, 1088, 800);
        init_podoboo(&podoboo[2], 7648, 1088, 800);
        init_podoboo(&podoboo[3], 7680, 1088, 800);
        init_podoboo(&podoboo[4], 7776, 1088, 800);
        init_podoboo(&podoboo[5], 7840, 1088, 800);
        init_podoboo(&podoboo[6], 7968, 1088, 800);
        init_podoboo(&podoboo[7], 8128, 1088, 800);
        init_podoboo(&podoboo[8], 8192, 1088, 800);
        init_podoboo(&podoboo[9], 8256, 1088, 800);
        init_podoboo(&podoboo[10], 8320, 1088, 800);

        // -- Coquilas --
        init_coquilas(&coquilas[0], 5896, 650);
        init_coquilas(&coquilas[1], 4416, 608);
        init_coquilas(&coquilas[2], 6272, 960);

        // -- Jean-Claude --
        init_jc(&jc[0], 10600, 928);
        init_jc(&jc[1], 10700, 450);
        init_jc(&jc[2], 11500, 750);
        init_jc(&jc[3], 13000, 928);
        
        // -- Drapeau fin --
        init_flag(flag, 15008, 747);

        // Coordonnées des pièces
        for (int i = 0; i < NB_PIECES; i++) { pieces[i].rect.w = 32; pieces[i].rect.h = 32; pieces[i].vivant = 1; }
        pieces[0].rect.x = 1088; pieces[0].rect.y = 1024;
        pieces[1].rect.x = 2144; pieces[1].rect.y = 444;
        pieces[2].rect.x = 4352; pieces[2].rect.y = 576;
        pieces[3].rect.x = 11585; pieces[3].rect.y = 640;

        // Checkpoints
        checkpoints[0].rect = (SDL_Rect){ 4096, 896, 64, 64 }; checkpoints[0].actif = 0;
        checkpoints[1].rect = (SDL_Rect){ 10176, 928, 64, 64 }; checkpoints[1].actif = 0;
    }

    // --- MAP 2 --- 
    else if(lvl->id == 2) {
        // Drapeau fin
        init_flag(flag, 20000, 1000);

        // -- Olafs --
        init_snowman(&olaf[0], 1440, 960);
        init_snowman(&olaf[1], 3040, 960);
        init_snowman(&olaf[2], 3552, 1056);
        init_snowman(&olaf[3], 3840, 1056);
        init_snowman(&olaf[4], 4000, 1056);
        init_snowman(&olaf[5], 4320, 1056);
        init_snowman(&olaf[6], 5344, 576);
        init_snowman(&olaf[7], 5408, 576);
        init_snowman(&olaf[8], 5472, 576);
        init_snowman(&olaf[9], 6496, 960);

        // -- Aliens --
        init_alien(&mesAliens[0], 7328, 960);
        init_alien(&mesAliens[1], 7520, 864);
        init_alien(&mesAliens[2], 8000, 928);
        init_alien(&mesAliens[3], 9088, 928);
        init_alien(&mesAliens[4], 10048, 1056);
        init_alien(&mesAliens[5], 10368, 1056);
        init_alien(&mesAliens[6], 10720, 864);
        init_alien(&mesAliens[7], 11360, 960);

        // -- Presses --
        init_presse(&presse[0], 14200, 850, 0);
        init_presse(&presse[1], 14400, 850, 200);
        init_presse(&presse[2], 14750, 850, 400);

        // Coordonnées des pièces
        for (int i = 0; i < NB_PIECES; i++) { pieces[i].rect.w = 32; pieces[i].rect.h = 32; pieces[i].vivant = 1; }
        pieces[0].rect.x = 5408; pieces[0].rect.y = 544;
        pieces[1].rect.x = 5536; pieces[1].rect.y = 896;
        pieces[2].rect.x = 40352; pieces[2].rect.y = 576;
        pieces[3].rect.x = 11585; pieces[3].rect.y = 640;

        // Checkpoints
        checkpoints[0].rect = (SDL_Rect){ 6720, 928, 64, 64 };
        checkpoints[0].actif = 0;
        checkpoints[1].rect = (SDL_Rect){ 11648, 896, 64, 64 };
        checkpoints[1].actif = 0;
    }
}

void draw_level_backgrounds(SDL_Renderer* renderer, Level* lvl, int cameraX, int cameraY, int screenW, int screenH) {
    if (lvl->id == 1) {
        // --- 1. LE BASIC ---
        draw_parallax_bg(renderer, lvl->bgs[0], cameraX, cameraY, 0.05f, 0.0f, screenW, screenH, 0, 0);   // sky
        draw_parallax_bg(renderer, lvl->bgs[3], cameraX, cameraY, 0.08f, 0.02f, screenW, screenH, 0, 50);  // cloud1
        draw_parallax_bg(renderer, lvl->bgs[4], cameraX, cameraY, 0.10f, 0.03f, screenW, screenH, 0, 150); // cloud2
        draw_parallax_bg(renderer, lvl->bgs[1], cameraX, cameraY, 0.15f, 0.05f, screenW, screenH, 0, 200); // mountain
        draw_parallax_bg(renderer, lvl->bgs[2], cameraX, cameraY, 0.40f, 0.1f, screenW, screenH, 0, 350);  // pine

        // --- 2. LE DONJON ---
        int startDonjonX = 128 * 32, endDonjonX = 318 * 32;
        SDL_Rect clipDonjon = { startDonjonX - cameraX, 0, endDonjonX - startDonjonX, screenH };
        SDL_RenderSetClipRect(renderer, &clipDonjon);
        draw_parallax_bg(renderer, lvl->bgs[5], cameraX, cameraY, 0.15f, 0.0f, screenW, screenH, startDonjonX, 0);
        SDL_RenderSetClipRect(renderer, NULL);

        // --- 3. LE DESERT ---
        int startDesertX = 318 * 32;
        int xStartEcran2 = startDesertX - cameraX;
        int biomeWidth = lvl->mapPixelWidth - startDesertX;

        SDL_Rect clipDesert = { (xStartEcran2 > 0 ? xStartEcran2 : 0), 0, screenW, screenH };
        SDL_RenderSetClipRect(renderer, &clipDesert);
        SDL_Rect destDesert = { xStartEcran2, 0, biomeWidth, screenH };
        SDL_RenderCopy(renderer, lvl->bgs[6], NULL, &destDesert);
        SDL_RenderSetClipRect(renderer, NULL);

        // --- 4. DECORS FIXES ---
        // Pyramide
        if (lvl->bgs[12]) {
            SDL_SetTextureAlphaMod(lvl->bgs[12], 76);
            SDL_Rect posP = { lvl->bgsPos[12].x - cameraX, screenH - 300 - 250, 400, 300 };
            SDL_RenderCopy(renderer, lvl->bgs[12], NULL, &posP);
        }

        // Statue
        if (lvl->bgs[11]) {
            SDL_SetTextureAlphaMod(lvl->bgs[11], 65);
            SDL_Rect posS = { lvl->bgsPos[11].x - cameraX, screenH - 250 - 250, 200, 250 };
            SDL_RenderCopy(renderer, lvl->bgs[11], NULL, &posS);
        }

        // Château
        if (lvl->bgs[13]) {
            SDL_Rect posC = lvl->bgsPos[13];
            posC.x -= cameraX;
            SDL_RenderCopy(renderer, lvl->bgs[13], NULL, &posC);
        }
    }

    // --- MAP 2 --- 
    else if(lvl->id == 2) {
        // --- HIVER ---
        int startWinterX = 0 * 32; 
        int endWinterX = 210 * 32;
        SDL_Rect clipWinter = { startWinterX - cameraX, 0, endWinterX - startWinterX, screenH };
        if (clipWinter.x < 0) { clipWinter.w += clipWinter.x; clipWinter.x = 0; } // Sécurité clipping

        SDL_RenderSetClipRect(renderer, &clipWinter);
        draw_parallax_bg(renderer, lvl->bgs[0]
            , cameraX, cameraY, 0.5f, 0.0f, screenW, screenH, startWinterX, 00);
        SDL_RenderSetClipRect(renderer, NULL);

        // --- LUNE ---
        int startMoonX = 210 * 32; 
        int endMoonX = 364 * 32; 
        SDL_Rect clipMoon = { startMoonX - cameraX, 0, endMoonX - startMoonX, screenH };
        
        if (clipMoon.x < 0) { clipMoon.w += clipMoon.x; clipMoon.x = 0; }
        
        if (clipMoon.w > 0) {
            SDL_RenderSetClipRect(renderer, &clipMoon);
            draw_parallax_bg(renderer, lvl->bgs[4], cameraX, cameraY, 0.62f, 0.0f, screenW, screenH, startMoonX, -130);
            draw_parallax_bg(renderer, lvl->bgs[3], cameraX, cameraY, 0.10f, 0.0f, screenW, screenH, startMoonX, 170);
            draw_parallax_bg(renderer, lvl->bgs[2], cameraX, cameraY, 0.25f, 0.0f, screenW, screenH, startMoonX, 265);
            draw_parallax_bg(renderer, lvl->bgs[1], cameraX, cameraY, 0.45f, 0.0f, screenW, screenH, startMoonX, 360);
            
            SDL_RenderSetClipRect(renderer, NULL);
        }
        // --- VILLE ---
        int startVilleX = 364 * 32;
        int endVilleX = MAP_WIDTH * 32;
        SDL_Rect clipVille = { startVilleX - cameraX, 0, endVilleX - startVilleX, screenH };
        if (clipVille.x < 0) { clipVille.w += clipVille.x; clipVille.x = 0; }

        SDL_RenderSetClipRect(renderer, &clipVille);
        draw_parallax_bg(renderer, lvl->bgs[4], cameraX, cameraY, 0.5f, 0.0f, screenW, screenH, startVilleX, 0);
        SDL_RenderSetClipRect(renderer, NULL);
    }
}

void cleanup_level(Level* lvl) {
    if (!lvl) return;
    free(lvl->tileMap);
    SDL_DestroyTexture(lvl->tileset.texture);
    for (int i = 0; i < 15; i++) if (lvl->bgs[i]) SDL_DestroyTexture(lvl->bgs[i]);
    free(lvl);
}