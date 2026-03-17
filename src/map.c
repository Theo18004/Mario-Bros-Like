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

    // grosse allocation pour eviter de crash 
    int max_line_length = 500000; 
    char* line = (char*)malloc(max_line_length * sizeof(char));
    
    if (line == NULL) {
        printf("Erreur: Plus de memoire disponible !\n");
        fclose(file);
        return 0;
    }

    int row = 0;
    while (fgets(line, max_line_length, file) && row < MAP_HEIGHT) {
        char* token = strtok(line, ",");
        int col = 0;
        while (token && col < MAP_WIDTH) {
            map_array[row * MAP_WIDTH + col] = atoi(token);
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    
    free(line);
    fclose(file);
    
    return 1;
}

void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY) {
    if (tileId < 0) return; 
    SDL_Rect src = { (tileId % ts->columns) * ts->tileWidth, (tileId / ts->columns) * ts->tileHeight, ts->tileWidth, ts->tileHeight };
    SDL_Rect dest = { x - scrollX, y - scrollY, ts->tileWidth * MAP_SCALE, ts->tileHeight * MAP_SCALE };
    SDL_RenderCopy(renderer, ts->texture, &src, &dest);
}


void draw_parallax_bg(SDL_Renderer* renderer, SDL_Texture* tex, int scrollX, int scrollY, float speedX, float speedY, int screenW, int screenH, int startX, int offsetY) {
    (void)screenH;
    if (!tex) return;

    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    int finalW = tw * MAP_SCALE; 
    int finalH = th * MAP_SCALE;
    int relativeX = (int)((startX - scrollX) * speedX);
    int posX = relativeX % finalW;
    if (posX > 0) posX -= finalW;
    int posY = offsetY - (int)(scrollY * speedY);

    SDL_Rect dest = { posX, posY, finalW, finalH };
    while (dest.x < screenW) {
        SDL_RenderCopy(renderer, tex, NULL, &dest);
        dest.x += finalW; 
    }
}