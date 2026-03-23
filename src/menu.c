#include "menu.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <stdio.h>

#define BTN_SOURCE_SIZE 250
#define COLONNES 12
#define LIGNES 4

int luminosite = 255; // valeur max : 255
int volume = 50; // valeur max 100

// permet de sauvegarder les parametres
void sauvegarder_parametres() {
    FILE* fichier = fopen("config.txt", "w"); // "w" pour write (écraser/créer)
    if (fichier != NULL) {
        fprintf(fichier, "%d %d", volume, luminosite);
        fclose(fichier);
        printf("Paramètres sauvegardés : Vol=%d, Lum=%d\n", volume, luminosite);
    }
}

// charge les parametres sauvegarder
void charger_parametres() {
    FILE* fichier = fopen("config.txt", "r"); // "r" pour read (lire)
    if (fichier != NULL) {
        // On lit les deux entiers dans le fichier
        if (fscanf(fichier, "%d %d", &volume, &luminosite) != 2) {
            // Si le fichier est corrompu, on met des valeurs par défaut
            volume = 50;
            luminosite = 255;
        }
        fclose(fichier);
        printf("Parametres charges depuis le fichier.\n");
    } else {
        // Si le fichier n'existe pas encore (premier lancement)
        volume = 50;
        luminosite = 255;
    }
}

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
                    afficher_parametres(renderer, width, height, font_boutons, texBG);
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

        // --- APPLIQUER LA LUMINOSITÉ PARTOUT ---
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite); 
        SDL_Rect fullScreen = {0, 0, width, height};
        SDL_RenderFillRect(renderer, &fullScreen);

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


void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG) {
    int paramRunning = 1;
    SDL_Event e;
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color or_couleur = {255, 215, 0, 255};

    // 1. Préparation des Textures de texte
    SDL_Rect tRectTitre, tRectSon, tRectLum, tRectRetour;
    SDL_Texture* txtTitre  = loadText(renderer, font, "REGLAGES", or_couleur, &tRectTitre);
    SDL_Texture* txtSon    = loadText(renderer, font, "SON", blanc, &tRectSon);
    SDL_Texture* txtLum    = loadText(renderer, font, "LUMINOSITE", blanc, &tRectLum);
    SDL_Texture* txtRetour = loadText(renderer, font, "RETOUR", blanc, &tRectRetour);

    // 2. Positions des éléments
    int centerX = width / 2;
    SDL_Rect posTitre  = { centerX - tRectTitre.w/2, 50, tRectTitre.w, tRectTitre.h };
    
    // Barres de réglage
    SDL_Rect barSon    = { centerX - 100, 200, 200, 25 };
    SDL_Rect barLum    = { centerX - 100, 320, 200, 25 };
    
    // Position du texte au-dessus des barres
    SDL_Rect posSon    = { centerX - tRectSon.w/2, 160, tRectSon.w, tRectSon.h };
    SDL_Rect posLum    = { centerX - tRectLum.w/2, 280, tRectLum.w, tRectLum.h };

    // Bouton Retour (on peut réutiliser l'image du bouton bois ici si on veut)
    SDL_Rect rectBtnRetour = { centerX - 100, 450, 200, 60 };
    SDL_Rect posRetourTxt  = { rectBtnRetour.x + (rectBtnRetour.w - tRectRetour.w)/2, 
                               rectBtnRetour.y + (rectBtnRetour.h - tRectRetour.h)/2, 
                               tRectRetour.w, tRectRetour.h };

    while (paramRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) paramRunning = 0;

            // Gestion des clics et du "drag" (glisser la souris)
            if (e.type == SDL_MOUSEBUTTONDOWN || (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)))) {
                SDL_Point p = { e.motion.x, e.motion.y };

                if (SDL_PointInRect(&p, &barSon)) {
                    // Calcul du pourcentage (0 à 100)
                    volume = ((p.x - barSon.x) * 100) / barSon.w;
                }
                if (SDL_PointInRect(&p, &barLum)) {
                    // Calcul de la valeur (0 à 255)
                    luminosite = ((p.x - barLum.x) * 255) / barLum.w;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN && SDL_PointInRect(&p, &rectBtnRetour)) {
                    paramRunning = 0;
                }
                if (SDL_PointInRect(&p, &rectBtnRetour)) {
                    sauvegarder_parametres();
                    paramRunning = 0; // On quitte la page de réglages
                }
            }
        }

        // --- DESSIN ---
        SDL_RenderClear(renderer);
        if (texBG) SDL_RenderCopy(renderer, texBG, NULL, NULL);

        // Titre et labels
        SDL_RenderCopy(renderer, txtTitre, NULL, &posTitre);
        SDL_RenderCopy(renderer, txtSon, NULL, &posSon);
        SDL_RenderCopy(renderer, txtLum, NULL, &posLum);

        // Fond des barres
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderFillRect(renderer, &barSon);
        SDL_RenderFillRect(renderer, &barLum);

        // Remplissage des barres
        SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Vert
        SDL_Rect fillSon = { barSon.x, barSon.y, (volume * barSon.w) / 100, barSon.h };
        SDL_RenderFillRect(renderer, &fillSon);

        SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Bleu
        SDL_Rect fillLum = { barLum.x, barLum.y, (luminosite * barLum.w) / 255, barLum.h };
        SDL_RenderFillRect(renderer, &fillLum);

        // Bouton Retour (Rectangle simple ou texture bouton_bois)
        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Marron
        SDL_RenderFillRect(renderer, &rectBtnRetour);
        SDL_RenderCopy(renderer, txtRetour, NULL, &posRetourTxt);

        // --- EFFET LUMINOSITÉ ---
        // On dessine un rectangle noir sur TOUT l'écran avec une transparence inverse
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite); 
        SDL_Rect fullScreen = {0, 0, width, height};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_RenderPresent(renderer);
    }

    // Nettoyage des textures locales
    SDL_DestroyTexture(txtTitre);
    SDL_DestroyTexture(txtSon);
    SDL_DestroyTexture(txtLum);
    SDL_DestroyTexture(txtRetour);
}

MenuResult afficher_pause(SDL_Renderer* renderer, int width, int height, Mix_Chunk* sfx) {
    SDL_Texture* texBouton = IMG_LoadTexture(renderer, "assets/bouton.png");
    SDL_Texture* texBG = IMG_LoadTexture(renderer, "assets/background.png");
    
    if (!texBouton) return MENU_PLAY;

    int imgW, imgH;
    SDL_QueryTexture(texBouton, NULL, NULL, &imgW, &imgH);
    int bw = imgW / COLONNES;
    int bh = imgH / LIGNES;

    // Sélection des icônes (Ligne 3)
    SDL_Rect srcPlay    = { bw * 4,  bh * 2, bw, bh }; 
    SDL_Rect srcRestart = { bw * 3,  bh * 2, bw, bh }; 
    SDL_Rect srcHome    = { bw * 6,  bh * 3 - 50, bw, bh }; 

    int displaySize = 100;
    int spacing = 40;
    int startX = (width - (displaySize * 3 + spacing * 2)) / 2;
    // On remonte les boutons (Centre - 150 pixels)
    int startY = (height / 2) - (displaySize / 2) - 150; 

    SDL_Rect dPlay    = { startX, startY, displaySize, displaySize };
    SDL_Rect dRestart = { startX + displaySize + spacing, startY, displaySize, displaySize };
    SDL_Rect dHome    = { startX + (displaySize + spacing) * 2, startY, displaySize, displaySize };

    int pauseRunning = 1;
    MenuResult result = MENU_PLAY;
    SDL_Event e;

    while (pauseRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { result = MENU_QUIT; pauseRunning = 0; }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point p = { e.button.x, e.button.y };
                if (SDL_PointInRect(&p, &dPlay)) { 
                    if(sfx) Mix_PlayChannel(-1, sfx, 0); 
                    pauseRunning = 0; 
                }
                if (SDL_PointInRect(&p, &dRestart)) { 
                    if(sfx) Mix_PlayChannel(-1, sfx, 0); 
                    result = MENU_OPTIONS; // Signal pour Restart
                    pauseRunning = 0; 
                }
                if (SDL_PointInRect(&p, &dHome)) { 
                    if(sfx) Mix_PlayChannel(-1, sfx, 0); 
                    result = MENU_QUIT; // Retour Menu Principal
                    pauseRunning = 0; 
                }
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) pauseRunning = 0;
        }

        SDL_RenderClear(renderer);
        if (texBG) SDL_RenderCopy(renderer, texBG, NULL, NULL);

        // Voile sombre léger pour voir le fond
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_RenderFillRect(renderer, NULL);

        SDL_RenderCopy(renderer, texBouton, &srcPlay, &dPlay);
        SDL_RenderCopy(renderer, texBouton, &srcRestart, &dRestart);
        SDL_RenderCopy(renderer, texBouton, &srcHome, &dHome);
        SDL_RenderPresent(renderer);
    }

    if(texBG) SDL_DestroyTexture(texBG);
    SDL_DestroyTexture(texBouton);
    return result;
}