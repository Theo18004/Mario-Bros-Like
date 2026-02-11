/**
 * @file map.c
 * @brief Implémentation du chargement de la carte.
 * @internal
 * Ce fichier contient la logique de lecture des pixels de l'image source 
 * et leur conversion en entités de jeu (plateformes et joueur).
 */

#include "../lib/map.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

void loadMapFromImage(const char* filename, SDL_Rect** platforms, int* count, SDL_Rect* playerPos) {
    // 1. Chargement de l'image
    SDL_Surface* mapSurface = IMG_Load(filename);
    if (!mapSurface) {
        printf("ERREUR : Impossible de charger la map '%s' : %s\n", filename, IMG_GetError());
        return;
    }

    // 2. Conversion au format RGBA standard pour lire les pixels sans erreur
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(mapSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(mapSurface); // On libère l'originale qui ne sert plus

    if (!formattedSurface) {
        printf("ERREUR : Echec de conversion du format de l'image.\n");
        return;
    }

    Uint32* pixels = (Uint32*)formattedSurface->pixels;
    int w = formattedSurface->w;
    int h = formattedSurface->h;

    // 3. Allocation de la mémoire pour le tableau de plateformes
    // On alloue le maximum possible (w * h), on pourrait optimiser plus tard
    *platforms = malloc(sizeof(SDL_Rect) * (w * h));
    if (*platforms == NULL) {
        printf("ERREUR : Echec d'allocation mémoire pour les plateformes.\n");
        SDL_FreeSurface(formattedSurface);
        return;
    }
    
    *count = 0;

    // 4. Analyse des pixels (Ligne par ligne, puis colonne par colonne)
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Uint32 pixel = pixels[y * w + x];
            
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, formattedSurface->format, &r, &g, &b, &a);

            // NOIR (0,0,0) -> Création d'un bloc de mur
            if (r == 0 && g == 0 && b == 0) {
                (*platforms)[*count].x = x * TILE_SIZE;
                (*platforms)[*count].y = y * TILE_SIZE;
                (*platforms)[*count].w = TILE_SIZE;
                (*platforms)[*count].h = TILE_SIZE;
                (*count)++;
            }
            // BLEU (0,0,255) -> Position initiale du joueur
            else if (r == 0 && g == 0 && b == 255) {
                playerPos->x = x * TILE_SIZE;
                playerPos->y = y * TILE_SIZE;
            }
        }
    }

    // 5. Nettoyage et rapport final
    SDL_FreeSurface(formattedSurface);
    printf("--- CHARGEMENT MAP TERMINE ---\n");
    printf("Fichier : %s (%dx%d pixels)\n", filename, w, h);
    printf("Nombre de plateformes creees : %d\n", *count);
    printf("Position joueur : x=%d, y=%d\n", playerPos->x, playerPos->y);
    printf("------------------------------\n");
}