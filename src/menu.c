#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Configuration
const int SCREEN_W = 800;
const int SCREEN_H = 600;

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init(); // On initialise le texte

    SDL_Window* window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 1. Chargement du décor et du bouton
    SDL_Texture* texBG = IMG_LoadTexture(renderer, "../assets/background.png");
    SDL_Texture* texBouton = IMG_LoadTexture(renderer, "../assets/bouton_bois.png");

    // Pour le bouton en bois
    SDL_SetTextureBlendMode(texBouton, SDL_BLENDMODE_BLEND);

    // 2. Positions (Empilés au centre comme sur ta photo)
    SDL_Rect rectJouer = { SCREEN_W/2 - 100, 50, 200, 60 };
    SDL_Rect rectParam = { SCREEN_W/2 - 100, 200, 200, 60 };
    SDL_Rect rectQuitter = { SCREEN_W/2 - 100, 420, 200, 60 };


    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            // Ajoute ici la détection de clic sur les Rect...
        }

        SDL_RenderClear(renderer);

        // --- DESSIN ---
        // A. L'arrière-plan (couvre tout l'écran)
        SDL_RenderCopy(renderer, texBG, NULL, NULL);

        // B. Les boutons (on réutilise la même texture de bois)
        SDL_RenderCopy(renderer, texBouton, NULL, &rectJouer);
        SDL_RenderCopy(renderer, texBouton, NULL, &rectParam);
        SDL_RenderCopy(renderer, texBouton, NULL, &rectQuitter);

        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    SDL_DestroyTexture(texBG);
    SDL_DestroyTexture(texBouton);
    // ...
    return 0;
}