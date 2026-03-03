#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "defs.h"

int load_map_from_csv(const char* filename, int* map_array) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur: Impossible d'ouvrir %s\n", filename);
        return 0;
    }
    char line[10240]; 
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < MAP_HEIGHT) {
        char* token = strtok(line, ",");
        int col = 0;
        while (token && col < MAP_WIDTH) {
            map_array[row * MAP_WIDTH + col] = atoi(token);
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    fclose(file);
    return 1;
}

void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY) {
    if (tileId < 0) return; 
    SDL_Rect src = { (tileId % ts->columns) * ts->tileWidth, (tileId / ts->columns) * ts->tileHeight, ts->tileWidth, ts->tileHeight };
    SDL_Rect dest = { x - scrollX, y - scrollY, ts->tileWidth * MAP_SCALE, ts->tileHeight * MAP_SCALE };
    SDL_RenderCopy(renderer, ts->texture, &src, &dest);
}

void draw_parallax_bg(SDL_Renderer* renderer, SDL_Texture* tex, int scrollX, int scrollY, float speedX, float speedY, int screenW, int screenH, int offsetY) {
    (void)screenH;
    if (!tex) return;
    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    int finalW = tw * MAP_SCALE;
    int finalH = th * MAP_SCALE;
    int posX = -(int)(scrollX * speedX) % finalW;
    int posY = offsetY - (int)(scrollY * speedY);
    SDL_Rect dest = { posX, posY, finalW, finalH };
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    
    // Répétition pour écrans larges
    if (dest.x + finalW < screenW) {
        dest.x += finalW;
        SDL_RenderCopy(renderer, tex, NULL, &dest);
        if (dest.x + finalW < screenW) { 
             dest.x += finalW;
             SDL_RenderCopy(renderer, tex, NULL, &dest);
        }
    }
}