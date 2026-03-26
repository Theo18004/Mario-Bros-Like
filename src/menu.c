#include "menu.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <stdio.h>

int luminosite = 255; // valeur max : 255
int volume = 50; // valeur max 100
int hitboxes = 0; // 0 = OFF, 1 = ON


// permet de sauvegarder les parametres
void sauvegarder_parametres() {
    FILE* fichier = fopen("config.txt", "w"); // "w" pour write (écraser/créer)
    if (fichier != NULL) {
        fprintf(fichier, "%d %d %d", volume, luminosite, hitboxes);
        fclose(fichier);
        printf("Paramètres sauvegardés : Vol=%d, Lum=%d, Hitboxes=%d\n", volume, luminosite, hitboxes);
    }
}

// charge les parametres sauvegarder
void charger_parametres() {
    FILE* fichier = fopen("config.txt", "r"); // "r" pour read (lire)
    if (fichier != NULL) {
        // On lit les deux entiers dans le fichier
        if (fscanf(fichier, "%d %d %d", &volume, &luminosite, &hitboxes) != 3) {
            // Si le fichier est corrompu, on met des valeurs par défaut
            volume = 50;
            luminosite = 255;
            hitboxes = 0;
        }
        fclose(fichier);
        printf("Parametres charges depuis le fichier.\n");
    } else {
        // Si le fichier n'existe pas encore (premier lancement)
        volume = 50;
        luminosite = 255;
        hitboxes = 0;
    }
}


// --- Fonction Utilitaire pour charger du texte rapidement ---
SDL_Texture* loadText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* rect) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) return NULL;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    return texture;
}


// ==============================================================
// ==================== AFFICHER MENU ===========================
// ==============================================================

MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height) {
    int menuRunning = 1;
    MenuResult result = MENU_QUIT;
    SDL_Event e;

    TTF_Font* font_titre = TTF_OpenFont("assets/NSuperMario.ttf", 64);
    TTF_Font* font_boutons = TTF_OpenFont("assets/NSuperMario.ttf", 32);

    SDL_Texture* texBG = IMG_LoadTexture(renderer, "assets/background.png");
    SDL_Texture* texBG2 = IMG_LoadTexture(renderer, "assets/background2.png");
    SDL_Texture* texBoutonBois = IMG_LoadTexture(renderer, "assets/bouton_bois.png");

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color or_couleur = {255, 215, 0, 255};

    SDL_Rect tRectTitre, tRectJouer, tRectP, tRectQ;
    SDL_Texture* txtTitre = loadText(renderer, font_titre, "MARIO-BROS-LIKE", or_couleur, &tRectTitre);
    SDL_Texture* txtJouer = loadText(renderer, font_boutons, "JOUER", blanc, &tRectJouer);
    SDL_Texture* txtParam = loadText(renderer, font_boutons, "PARAMETRES", blanc, &tRectP);
    SDL_Texture* txtQuit = loadText(renderer, font_boutons, "QUITTER", blanc, &tRectQ);

    int btnW = 300; int btnH = 70; int centerX = width / 2;
    SDL_Rect rectJouer = { centerX - btnW/2, height / 2 - 200, btnW, btnH }; 
    SDL_Rect rectParam = { centerX - btnW/2, height / 2 - 100,  btnW, btnH }; 
    SDL_Rect rectQuit  = { centerX - btnW/2, height / 2 ,  btnW, btnH }; 
    SDL_Rect posTitre = { centerX - tRectTitre.w/2, height/10, tRectTitre.w, tRectTitre.h };

    while (menuRunning) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePoint = {mouseX, mouseY};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) menuRunning = 0;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                // --- Bouton JOUER ---
                if (SDL_PointInRect(&mousePoint, &rectJouer)) {
                    MenuResult choix = afficher_selection_map(renderer, width, height, font_boutons, texBG2);
                    if (choix == MENU_CHOIX_MAP1 || choix == MENU_CHOIX_MAP2) {
                        result = choix;
                        menuRunning = 0;
                    } else if (choix == MENU_QUIT) {
                        result = MENU_QUIT;
                        menuRunning = 0;
                    }
                }
                if (SDL_PointInRect(&mousePoint, &rectParam)) {
                    afficher_parametres(renderer, width, height, font_boutons, texBG);
                }
                if (SDL_PointInRect(&mousePoint, &rectQuit)) {
                    result = MENU_QUIT;
                    menuRunning = 0;
                }
            }
        }

        SDL_RenderClear(renderer);
        if (texBG) SDL_RenderCopy(renderer, texBG, NULL, NULL);
        SDL_RenderCopy(renderer, txtTitre, NULL, &posTitre);

        // --- DESSIN DU BOUTON JOUER ---
        if (texBoutonBois) SDL_RenderCopy(renderer, texBoutonBois, NULL, &rectJouer);
        SDL_Rect posTxtJ = { rectJouer.x + (btnW - tRectJouer.w)/2, rectJouer.y + (btnH - tRectJouer.h)/2, tRectJouer.w, tRectJouer.h };
        SDL_RenderCopy(renderer, txtJouer, NULL, &posTxtJ);

        // Dessin du bouton PARAMETRES
        if (texBoutonBois) SDL_RenderCopy(renderer, texBoutonBois, NULL, &rectParam);
        SDL_Rect posTxtP = { rectParam.x + (btnW - tRectP.w)/2, rectParam.y + (btnH - tRectP.h)/2, tRectP.w, tRectP.h };
        SDL_RenderCopy(renderer, txtParam, NULL, &posTxtP);

        // Dessin du bouton QUITTER
        if (texBoutonBois) SDL_RenderCopy(renderer, texBoutonBois, NULL, &rectQuit);
        SDL_Rect posTxtQ = { rectQuit.x + (btnW - tRectQ.w)/2, rectQuit.y + (btnH - tRectQ.h)/2, tRectQ.w, tRectQ.h };
        SDL_RenderCopy(renderer, txtQuit, NULL, &posTxtQ);

        // Effet survol
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        if (SDL_PointInRect(&mousePoint, &rectJouer)) SDL_RenderFillRect(renderer, &rectJouer);
        if (SDL_PointInRect(&mousePoint, &rectParam)) SDL_RenderFillRect(renderer, &rectParam);
        if (SDL_PointInRect(&mousePoint, &rectQuit)) SDL_RenderFillRect(renderer, &rectQuit);

        // Luminosité
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite); 
        SDL_Rect fullScreen = {0, 0, width, height};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(txtTitre); SDL_DestroyTexture(txtJouer); SDL_DestroyTexture(txtParam); SDL_DestroyTexture(txtQuit);
    SDL_DestroyTexture(texBG); SDL_DestroyTexture(texBoutonBois);
    TTF_CloseFont(font_titre); TTF_CloseFont(font_boutons);
    return result;
}


void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG) {
    int paramRunning = 1;
    SDL_Event e;
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color or_couleur = {255, 215, 0, 255};

    // Préparation des Textures de texte
    SDL_Rect tRectTitre, tRectSon, tRectLum, tRectRetour;
    SDL_Texture* txtTitre  = loadText(renderer, font, "REGLAGES", or_couleur, &tRectTitre);
    SDL_Texture* txtSon    = loadText(renderer, font, "SON", blanc, &tRectSon);
    SDL_Texture* txtLum    = loadText(renderer, font, "LUMINOSITE", blanc, &tRectLum);
    SDL_Texture* txtRetour = loadText(renderer, font, "RETOUR", blanc, &tRectRetour);

    // Positions des éléments
    int centerX = width / 2;
    SDL_Rect posTitre  = { centerX - tRectTitre.w/2, 50, tRectTitre.w, tRectTitre.h };
    
    // Barres de réglage
    SDL_Rect barSon    = { centerX - 100, 200, 200, 25 };
    SDL_Rect barLum    = { centerX - 100, 320, 200, 25 };
    
    // Position du texte au-dessus des barres
    SDL_Rect posSon    = { centerX - tRectSon.w/2, 160, tRectSon.w, tRectSon.h };
    SDL_Rect posLum    = { centerX - tRectLum.w/2, 280, tRectLum.w, tRectLum.h };

    // --- LE SYSTÈME HITBOX ---
    // Rectangle de position du texte "Afficher Hitboxes"
    SDL_Rect rectTxtHitbox = { centerX - 100, 360, 200, 25 }; 
    
    // Rectangle du switch ON/OFF
    SDL_Rect rectSwitchHitbox = { centerX - 60, 390, 120, 30 }; 

    // Bouton Retour 
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
                // Glisser les barres
                if (SDL_PointInRect(&p, &barSon)) {
                    volume = ((p.x - barSon.x) * 100) / barSon.w;
                }
                if (SDL_PointInRect(&p, &barLum)) {
                    luminosite = ((p.x - barLum.x) * 255) / barLum.w;
                }
                // Clics uniques sur les boutons
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (SDL_PointInRect(&p, &rectSwitchHitbox)) {
                        hitboxes = !hitboxes; // Inverse ON / OFF
                    }
                    if (SDL_PointInRect(&p, &rectBtnRetour)) {
                        sauvegarder_parametres();
                        paramRunning = 0; // On quitte
                    }
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

        // --- AFFICHAGE HITBOXES ---
        // Dessiner le texte "Afficher Hitboxes"
        SDL_Rect tRectHitTxt;
        SDL_Texture* txtHitTxt = loadText(renderer, font, "AFFICHER HITBOXES", blanc, &tRectHitTxt);
        if (txtHitTxt != NULL) {
            // Centrer le texte au-dessus de son rectangle de position
            SDL_Rect posHitTxtFixed = { rectTxtHitbox.x + (rectTxtHitbox.w - tRectHitTxt.w)/2,
                                         rectTxtHitbox.y + (rectTxtHitbox.h - tRectHitTxt.h)/2,
                                         tRectHitTxt.w, tRectHitTxt.h };
            SDL_RenderCopy(renderer, txtHitTxt, NULL, &posHitTxtFixed);
            SDL_DestroyTexture(txtHitTxt); 
        }

        // Dessiner le fond du Switch 
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &rectSwitchHitbox);

        if (hitboxes) {
            SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Vert pour ON
        } else {
            SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255); // Rouge pour OFF
        }
        SDL_Rect switchInner = { rectSwitchHitbox.x + 2, rectSwitchHitbox.y + 2,
                                rectSwitchHitbox.w - 4, rectSwitchHitbox.h - 4 };
        SDL_RenderFillRect(renderer, &switchInner);

        // Dessiner le curseur du switch
        int curseurW = rectSwitchHitbox.w / 3; // Le curseur fait 1/3 de la barre
        SDL_Rect rectCurseur;
        rectCurseur.w = curseurW;
        rectCurseur.h = rectSwitchHitbox.h + 6; // Dépasse légèrement
        rectCurseur.y = rectSwitchHitbox.y - 3; // Centré en hauteur

        if (hitboxes) {
            // Curseur à droite pour ON
            rectCurseur.x = rectSwitchHitbox.x + rectSwitchHitbox.w - curseurW - 2;
        } else {
            // Curseur à gauche pour OFF
            rectCurseur.x = rectSwitchHitbox.x + 2;
        }

        // Dessin du curseur 
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Ombre du curseur
        SDL_RenderFillRect(renderer, &rectCurseur);
        
        SDL_Rect curseurInner = { rectCurseur.x + 2, rectCurseur.y + 2,
                                  rectCurseur.w - 4, rectCurseur.h - 4 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
        SDL_RenderFillRect(renderer, &curseurInner);

        // Bouton Retour 
        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Marron
        SDL_RenderFillRect(renderer, &rectBtnRetour);
        SDL_RenderCopy(renderer, txtRetour, NULL, &posRetourTxt);

        // --- EFFET LUMINOSITÉ ---
        // On dessine un rectangle noir sur l'écran avec une transparence inverse
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


MenuResult afficher_selection_map(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG2) {
    int selectionRunning = 1;
    MenuResult result = MENU_NONE;
    SDL_Event e;
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color or_couleur = {255, 215, 0, 255};

    TTF_Font* font_titre = TTF_OpenFont("assets/NSuperMario.ttf", 64);
    SDL_Texture* texBoutonBois = IMG_LoadTexture(renderer, "assets/bouton_bois.png");

    // --- DÉCLARATIONS ---
    int centerX = width / 2;
    int size = 250; 
    int btnW = 300; 
    int btnH = 70;

    // Positions des zones cliquables
    SDL_Rect rectMap1 = { centerX - 440, height - 470, size, size };
    SDL_Rect rectMap2 = { centerX - 90, height - 470, size, size };
    SDL_Rect rectRetour = { centerX - btnW / 2 + 10, height - 100, btnW, btnH };

    // Textes (Titre et Retour)
    SDL_Rect tRectRetour, tRectTitre;
    SDL_Texture* txtRetour = loadText(renderer, font, "RETOUR", blanc, &tRectRetour);
    SDL_Texture* txtTitre = loadText(renderer, font_titre, "MARIO-BROS-LIKE", or_couleur, &tRectTitre);

    // Position du titre en haut
    SDL_Rect posTitre = { centerX - tRectTitre.w/2, height/10, tRectTitre.w, tRectTitre.h };

    while (selectionRunning) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mP = {mx, my};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_DestroyTexture(texBoutonBois);
                SDL_DestroyTexture(txtTitre);
                SDL_DestroyTexture(txtRetour);
                TTF_CloseFont(font_titre);
                return MENU_QUIT;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                selectionRunning = 0; 
            }
            
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&mP, &rectMap1)) {
                    result = MENU_CHOIX_MAP1;
                    selectionRunning = 0;
                }
                if (SDL_PointInRect(&mP, &rectMap2)) {
                    result = MENU_CHOIX_MAP2;
                    selectionRunning = 0;
                }
                if (SDL_PointInRect(&mP, &rectRetour)) {
                    result = MENU_NONE; 
                    selectionRunning = 0;
                }
            }
        }

        SDL_RenderClear(renderer);
        
        // Image de fond
        if (texBG2) SDL_RenderCopy(renderer, texBG2, NULL, NULL);

        // Titre du jeu
        SDL_RenderCopy(renderer, txtTitre, NULL, &posTitre);

        // Bouton Retour
        if (texBoutonBois) SDL_RenderCopy(renderer, texBoutonBois, NULL, &rectRetour);
        SDL_Rect posTxtRetour = { rectRetour.x + (btnW - tRectRetour.w)/2, 
                                   rectRetour.y + (btnH - tRectRetour.h)/2, 
                                   tRectRetour.w, tRectRetour.h };
        SDL_RenderCopy(renderer, txtRetour, NULL, &posTxtRetour);

        if (SDL_PointInRect(&mP, &rectRetour)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
            SDL_RenderFillRect(renderer, &rectRetour);
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyage local
    SDL_DestroyTexture(txtRetour);
    SDL_DestroyTexture(txtTitre);
    SDL_DestroyTexture(texBoutonBois);
    TTF_CloseFont(font_titre);

    return result;
}