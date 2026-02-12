#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Charge les IDs du fichier CSV dans le tableau map_array.
 * Accepte les virgules et les points-virgules comme séparateurs.
 */
int load_map_from_csv(const char* filename, int* map_array) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier de map : %s\n", filename);
        return 0;
    }

    char line[4096];
    int row = 0;

    // Initialisation du tableau avec du vide (-1) par sécurité
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        map_array[i] = -1;
    }

    while (fgets(line, sizeof(line), file) && row < MAP_HEIGHT) {
        // strtok gère les virgules, points-virgules et retours à la ligne
        char* token = strtok(line, ",;\n\r");
        int col = 0;

        while (token && col < MAP_WIDTH) {
            map_array[row * MAP_WIDTH + col] = atoi(token);
            token = strtok(NULL, ",;\n\r");
            col++;
        }
        row++;
    }

    fclose(file);
    printf("Map chargee avec succes (%d lignes lues).\n", row);
    return 1;
}

/**
 * Dessine une tuile individuelle en calculant sa position dans le tileset.
 */
void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY) {
    // Si l'ID est -1 (vide), on ne dessine rien
    if (tileId < 0) return;

    // Calcul de la position de la tuile dans l'image source (Terrain.png)
    // ts->columns est le nombre de tuiles par ligne dans ton image
    SDL_Rect src = {
        (tileId % ts->columns) * ts->tileWidth,
        (tileId / ts->columns) * ts->tileHeight,
        ts->tileWidth,
        ts->tileHeight
    };

    // Calcul de la position sur l'écran avec le décalage de la caméra
    SDL_Rect dest = {
        x - scrollX,
        y - scrollY,
        ts->tileWidth * MAP_SCALE,
        ts->tileHeight * MAP_SCALE
    };

    SDL_RenderCopy(renderer, ts->texture, &src, &dest);
}

/**
 * Parcourt toute la grille et dessine chaque tuile.
 */
void render_world(SDL_Renderer* renderer, int* map_array, Tileset* ts, int scrollX, int scrollY) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tileId = map_array[y * MAP_WIDTH + x];
            
            // On ne dessine que si ce n'est pas du vide
            if (tileId != -1) {
                draw_tile(renderer, ts, tileId, 
                          x * TILE_SIZE * MAP_SCALE, 
                          y * TILE_SIZE * MAP_SCALE, 
                          scrollX, scrollY);
            }
        }
    }
}