#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "ennemi.h"
#include "Items.h"
#include "flag.h"
#include "defs.h"
#include "player.h"

typedef struct {
    int id;
    int* tileMap;
    Tileset tileset;
    
    // Fonds & Décors 
    SDL_Texture* bgs[15];
    SDL_Rect bgsPos[15]; 
    
    SDL_Point playerStart;
    
    int mapPixelWidth;
    int mapPixelHeight;
} Level;

// Fonctions de gestion de map
Level* load_level(SDL_Renderer* renderer, int levelID);
void spawn_level_entities(Level* lvl, Ennemi* loupas, Thwomp* thwomps, Podoboo* podoboo, Coquilas* coquilas, Ennemi* jc, Ennemi* olaf,Presse* presse,Ennemi* mesAliens, Piece* pieces, Checkpoint* checkpoints, Flag* flag);
void draw_level_backgrounds(SDL_Renderer* renderer, Level* lvl, int cameraX, int cameraY, int screenW, int screenH);
void cleanup_level(Level* lvl);

#endif