#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "map.h"

int main() {
    // 1. Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Apercu de la Map", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 2. Chargement de la map
    SDL_Rect* worldPlatforms = NULL;
    int platformCount = 0;
    SDL_Rect playerPos = {0, 560, 40, 40}; // Taille par défaut

    loadMapFromImage("assets/map.png", &worldPlatforms, &platformCount, &playerPos);
    SDL_Texture* mapBackground = IMG_LoadTexture(renderer, "assets/map.png");

    // 3. Boucle d'affichage
    int running = 1;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        // Fond bleu ciel
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);
        SDL_Rect dest = {0, 0, 800, 600}; 
        SDL_RenderCopy(renderer, mapBackground, NULL, &dest);

        // Dessin des plateformes en NOIR
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < platformCount; i++) {
            SDL_RenderFillRect(renderer, &worldPlatforms[i]);
        }

        // Dessin du joueur en ROUGE
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &playerPos);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // 4. Nettoyage
    free(worldPlatforms);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}