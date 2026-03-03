#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

const int SCREEN_W = 800;
const int SCREEN_H = 600;

// Fonction utilitaire pour créer une texture de texte
SDL_Texture* loadText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* rect) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    // On ajuste le rect pour centrer le texte plus tard
    rect->w = surface->w;
    rect->h = surface->h;
    
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Menu Interactif", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 1. Chargement des ressources
    SDL_Texture* texBG = IMG_LoadTexture(renderer, "../assets/background.png");
    SDL_Texture* texBouton = IMG_LoadTexture(renderer, "../assets/bouton_bois.png");
    
    // Chargement de la police des boutons
    TTF_Font* font_boutons = TTF_OpenFont("../assets/police.ttf", 24); 

    // Chargement de la police du titre du jeu
    TTF_Font* font_titre = TTF_OpenFont("../assets/police.ttf", 40); 
    if (!font_titre || !font_boutons) printf("Erreur police: %s\n", TTF_GetError());

    SDL_Color blanc = {255, 255, 255, 255};

    // 2. Définition des zones des boutons
    SDL_Rect rectJouer = { SCREEN_W/2 - 100, 250, 200, 60 };
    SDL_Rect rectParam = { SCREEN_W/2 - 100, 350, 200, 60 };
    SDL_Rect rectQuitter = { SCREEN_W/2 - 100, 450, 200, 60 };

    // titre du jeu
    SDL_Rect tRectTitre;
    SDL_Color couleurTitre = {255, 215, 0, 255};
    SDL_Texture * txtTitre = loadText(renderer, font_titre, "MARIO-BROS-LIKE", couleurTitre, &tRectTitre);

    //positionnement du titre
    SDL_Rect posTitre = { SCREEN_W/2 - tRectTitre.w/2, 100, tRectTitre.w, tRectTitre.h };

    // 3. Création des textures de texte
    SDL_Rect tRectJ, tRectP, tRectQ;
    SDL_Texture* txtJouer = loadText(renderer, font_boutons, "JOUER", blanc, &tRectJ);
    SDL_Texture* txtParam = loadText(renderer, font_boutons, "PARAMETRES", blanc, &tRectP);
    SDL_Texture* txtQuitter = loadText(renderer, font_boutons, "QUITTER", blanc, &tRectQ);

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;

            // Détection du clic
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point mousePos = { e.button.x, e.button.y };

                if (SDL_PointInRect(&mousePos, &rectJouer)) {
                    printf("Lancement du jeu...\n");
                }
                else if (SDL_PointInRect(&mousePos, &rectParam)) {
                    printf("Ouverture des paramètres...\n");
                }
                else if (SDL_PointInRect(&mousePos, &rectQuitter)) {
                    running = 0;
                }
            }
        }

        SDL_RenderClear(renderer);

        // --- DESSIN ---
        SDL_RenderCopy(renderer, texBG, NULL, NULL);

        // Titre du jeu
        SDL_RenderCopy(renderer, txtTitre, NULL, &posTitre);

        // Dessin des boutons en bois
        SDL_RenderCopy(renderer, texBouton, NULL, &rectJouer);
        SDL_RenderCopy(renderer, texBouton, NULL, &rectParam);
        SDL_RenderCopy(renderer, texBouton, NULL, &rectQuitter);

        // Dessin du texte (centré sur les boutons)
        SDL_Rect posJ = { rectJouer.x + (rectJouer.w - tRectJ.w)/2, rectJouer.y + (rectJouer.h - tRectJ.h)/2, tRectJ.w, tRectJ.h };
        SDL_RenderCopy(renderer, txtJouer, NULL, &posJ);

        SDL_Rect posP = { rectParam.x + (rectParam.w - tRectP.w)/2, rectParam.y + (rectParam.h - tRectP.h)/2, tRectP.w, tRectP.h };
        SDL_RenderCopy(renderer, txtParam, NULL, &posP);

        SDL_Rect posQ = { rectQuitter.x + (rectQuitter.w - tRectQ.w)/2, rectQuitter.y + (rectQuitter.h - tRectQ.h)/2, tRectQ.w, tRectQ.h };
        SDL_RenderCopy(renderer, txtQuitter, NULL, &posQ);

        SDL_RenderPresent(renderer);
    }

    // Nettoyage complet
    SDL_DestroyTexture(txtJouer);
    SDL_DestroyTexture(txtParam);
    SDL_DestroyTexture(txtQuitter);
    TTF_CloseFont(font_titre);
    TTF_CloseFont(font_boutons);
    SDL_DestroyTexture(texBG);
    SDL_DestroyTexture(texBouton);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}