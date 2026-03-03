#include "menu.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Fonction utilitaire interne
SDL_Texture* loadText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* rect) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return NULL;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;
    
    SDL_FreeSurface(surface);
    return texture;
}

MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height) {
    // Note: On suppose que SDL, IMG et TTF sont déjà initialisés dans le main.c
    
    // Réinitialiser la taille logique pour que le menu soit net (pas pixelisé comme le jeu)
    SDL_RenderSetLogicalSize(renderer, width, height);

    // 1. Chargement des ressources
    // Attention aux chemins : j'ai harmonisé avec main.c (assets/ au lieu de ../assets/)
    SDL_Texture* texBG = IMG_LoadTexture(renderer, "assets/background.png"); 
    // Si pas de background, on met une couleur unie pour éviter le crash
    if (!texBG) SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    SDL_Texture* texBouton = IMG_LoadTexture(renderer, "assets/bouton_bois.png");
    
    TTF_Font* font_boutons = TTF_OpenFont("assets/police.ttf", 24); 
    TTF_Font* font_titre = TTF_OpenFont("assets/police.ttf", 40); 

    if (!font_titre || !font_boutons) {
        printf("Erreur chargement police menu: %s\n", TTF_GetError());
        // En cas d'erreur critique, on quitte
        return MENU_QUIT; 
    }

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color couleurTitre = {255, 215, 0, 255};

    // 2. Définition des zones (centrées par rapport à la taille width/height reçue)
    int btnW = 200;
    int btnH = 60;
    int centerX = width / 2;
    
    SDL_Rect rectJouer =   { centerX - btnW/2, 250, btnW, btnH };
    SDL_Rect rectParam =   { centerX - btnW/2, 350, btnW, btnH };
    SDL_Rect rectQuitter = { centerX - btnW/2, 450, btnW, btnH };

    // Titre
    SDL_Rect tRectTitre;
    SDL_Texture * txtTitre = loadText(renderer, font_titre, "MARIO-BROS-LIKE", couleurTitre, &tRectTitre);
    SDL_Rect posTitre = { centerX - tRectTitre.w/2, 100, tRectTitre.w, tRectTitre.h };

    // Textes boutons
    SDL_Rect tRectJ, tRectP, tRectQ;
    SDL_Texture* txtJouer = loadText(renderer, font_boutons, "JOUER", blanc, &tRectJ);
    SDL_Texture* txtParam = loadText(renderer, font_boutons, "PARAMETRES", blanc, &tRectP);
    SDL_Texture* txtQuitter = loadText(renderer, font_boutons, "QUITTER", blanc, &tRectQ);

    MenuResult result = MENU_QUIT; // Par défaut
    int menuRunning = 1;
    SDL_Event e;

    while (menuRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                result = MENU_QUIT;
                menuRunning = 0;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point mousePos = { e.button.x, e.button.y };

                if (SDL_PointInRect(&mousePos, &rectJouer)) {
                    result = MENU_PLAY;
                    menuRunning = 0;
                }
                else if (SDL_PointInRect(&mousePos, &rectParam)) {
                    printf("Options (Non implémenté)\n");
                    // Pour l'instant on ne fait rien, on reste dans le menu
                }
                else if (SDL_PointInRect(&mousePos, &rectQuitter)) {
                    result = MENU_QUIT;
                    menuRunning = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // --- DESSIN ---
        if (texBG) SDL_RenderCopy(renderer, texBG, NULL, NULL);

        SDL_RenderCopy(renderer, txtTitre, NULL, &posTitre);

        // Boutons
        if (texBouton) {
            SDL_RenderCopy(renderer, texBouton, NULL, &rectJouer);
            SDL_RenderCopy(renderer, texBouton, NULL, &rectParam);
            SDL_RenderCopy(renderer, texBouton, NULL, &rectQuitter);
        } else {
            // Fallback si pas de texture bouton : rectangles gris
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderFillRect(renderer, &rectJouer);
            SDL_RenderFillRect(renderer, &rectParam);
            SDL_RenderFillRect(renderer, &rectQuitter);
        }

        // Textes boutons centrés
        SDL_Rect posJ = { rectJouer.x + (rectJouer.w - tRectJ.w)/2, rectJouer.y + (rectJouer.h - tRectJ.h)/2, tRectJ.w, tRectJ.h };
        SDL_RenderCopy(renderer, txtJouer, NULL, &posJ);

        SDL_Rect posP = { rectParam.x + (rectParam.w - tRectP.w)/2, rectParam.y + (rectParam.h - tRectP.h)/2, tRectP.w, tRectP.h };
        SDL_RenderCopy(renderer, txtParam, NULL, &posP);

        SDL_Rect posQ = { rectQuitter.x + (rectQuitter.w - tRectQ.w)/2, rectQuitter.y + (rectQuitter.h - tRectQ.h)/2, tRectQ.w, tRectQ.h };
        SDL_RenderCopy(renderer, txtQuitter, NULL, &posQ);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyage local (on ne détruit PAS le renderer ni la fenêtre !)
    SDL_DestroyTexture(txtJouer); SDL_DestroyTexture(txtParam); SDL_DestroyTexture(txtQuitter);
    SDL_DestroyTexture(txtTitre);
    if(texBG) SDL_DestroyTexture(texBG);
    if(texBouton) SDL_DestroyTexture(texBouton);
    TTF_CloseFont(font_titre);
    TTF_CloseFont(font_boutons);

    return result;
}