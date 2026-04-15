/**
 * @file menu.c
 * @brief Système de menus (principal, options) et gestion des paramètres.
 */

#include "menu.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h> 
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
        // Si le fichier n'existe pas encore
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

// --- Fonction pour dessiner une touche de clavier carrée (style 3D) ---
static void drawKey(SDL_Renderer* renderer, int x, int y, SDL_Texture* texTxt, SDL_Rect txtRect) {
    int keySize = 50;

    // Ombre (Effet 3D)
    SDL_Rect shadow = {x + 3, y + 3, keySize, keySize};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &shadow);

    // Face de la touche (Blanc cassé)
    SDL_Rect box = {x, y, keySize, keySize};
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &box);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &box);
    
    // Texte centré dans la touche
    SDL_Rect dest = { x + (keySize - txtRect.w)/2, y + (keySize - txtRect.h)/2, txtRect.w, txtRect.h };
    SDL_RenderCopy(renderer, texTxt, NULL, &dest);
}

// --- Fonction pour dessiner une touche de clavier RECTANGULAIRE ---
static void drawWideKey(SDL_Renderer* renderer, int x, int y, int width, SDL_Texture* texTxt, SDL_Rect txtRect) {
    int keyHeight = 50;

    // Ombre (Effet 3D)
    SDL_Rect shadow = {x + 3, y + 3, width, keyHeight};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &shadow);

    // Face de la touche (Blanc cassé)
    SDL_Rect box = {x, y, width, keyHeight};
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &box);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &box);
    
    // Texte centré dans la touche
    SDL_Rect dest = { x + (width - txtRect.w)/2, y + (keyHeight - txtRect.h)/2, txtRect.w, txtRect.h };
    SDL_RenderCopy(renderer, texTxt, NULL, &dest);
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
        if (!menuRunning) break; 

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

        // Luminosité globale
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite); 
        SDL_Rect fullScreen = {0, 0, width, height};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(txtTitre); SDL_DestroyTexture(txtJouer); SDL_DestroyTexture(txtParam); SDL_DestroyTexture(txtQuit);
    SDL_DestroyTexture(texBG); SDL_DestroyTexture(texBG2); SDL_DestroyTexture(texBoutonBois);
    TTF_CloseFont(font_titre); TTF_CloseFont(font_boutons);
    return result;
}


// ==============================================================
// ================= AFFICHER PARAMETRES ========================
// ==============================================================

void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG) {
    int paramRunning = 1;
    SDL_Event e;
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color or_couleur = {255, 215, 0, 255};
    SDL_Color noir = {0, 0, 0, 255}; 

    SDL_Texture* texBoutonBois = IMG_LoadTexture(renderer, "assets/bouton_bois.png");

    // --- TEXTES RÉGLAGES ---
    SDL_Rect tRectTitre, tRectSon, tRectLum, tRectRetour;
    SDL_Texture* txtTitre  = loadText(renderer, font, "REGLAGES", or_couleur, &tRectTitre);
    SDL_Texture* txtSon    = loadText(renderer, font, "SON", blanc, &tRectSon);
    SDL_Texture* txtLum    = loadText(renderer, font, "LUMINOSITE", blanc, &tRectLum);
    SDL_Texture* txtRetour = loadText(renderer, font, "RETOUR", blanc, &tRectRetour);

    // --- TEXTES COMMANDES ---
    SDL_Rect tRectCmdTitre, tRectMouvDesc, tRectSautDesc, tRectPauseDesc, tRectQuitDesc, tRectOu;
    SDL_Texture* txtCmdTitre = loadText(renderer, font, "COMMANDES", or_couleur, &tRectCmdTitre);
    SDL_Texture* txtMouvDesc = loadText(renderer, font, "Deplacement :", blanc, &tRectMouvDesc);
    SDL_Texture* txtSautDesc = loadText(renderer, font, "Saut :", blanc, &tRectSautDesc);
    SDL_Texture* txtPauseDesc= loadText(renderer, font, "Pause :", blanc, &tRectPauseDesc);
    SDL_Texture* txtQuitDesc = loadText(renderer, font, "Quitter :", blanc, &tRectQuitDesc);
    SDL_Texture* txtOu       = loadText(renderer, font, "OU", blanc, &tRectOu);

    // --- TEXTES DES TOUCHES CLAVIER ---
    SDL_Rect tR_Z, tR_Q, tR_D, tR_P, tR_Echap, tR_Up, tR_Left, tR_Right;
    SDL_Texture* txtZ    = loadText(renderer, font, "Z", noir, &tR_Z);
    SDL_Texture* txtQ    = loadText(renderer, font, "Q", noir, &tR_Q);
    SDL_Texture* txtD    = loadText(renderer, font, "D", noir, &tR_D);
    SDL_Texture* txtP    = loadText(renderer, font, "P", noir, &tR_P);
    SDL_Texture* txtEchap= loadText(renderer, font, "ECHAP", noir, &tR_Echap);
    SDL_Texture* txtUp   = loadText(renderer, font, "^", noir, &tR_Up);
    SDL_Texture* txtLeft = loadText(renderer, font, "<", noir, &tR_Left);
    SDL_Texture* txtRight= loadText(renderer, font, ">", noir, &tR_Right);

    // Positions Globales (Deux colonnes)
    int centerX = width / 2;
    int leftX = centerX - 250;
    int rightX = centerX + 50; 

    SDL_Rect posTitre  = { centerX - tRectTitre.w/2, 50, tRectTitre.w, tRectTitre.h };
    
    // Position Colonne Gauche (Réglages)
    SDL_Rect posSon    = { leftX - tRectSon.w/2, 160, tRectSon.w, tRectSon.h };
    SDL_Rect barSon    = { leftX - 100, 200, 200, 25 };
    SDL_Rect posLum    = { leftX - tRectLum.w/2, 280, tRectLum.w, tRectLum.h };
    SDL_Rect barLum    = { leftX - 100, 320, 200, 25 };
    SDL_Rect rectTxtHitbox = { leftX - 100, 360, 200, 25 }; 
    SDL_Rect rectSwitchHitbox = { leftX - 60, 390, 120, 30 }; 

    // Bouton Retour 
    int btnW = 300, btnH = 70;
    SDL_Rect rectBtnRetour = { centerX - btnW/2, 550, btnW, btnH };
    SDL_Rect posRetourTxt  = { rectBtnRetour.x + (rectBtnRetour.w - tRectRetour.w)/2, 
                               rectBtnRetour.y + (rectBtnRetour.h - tRectRetour.h)/2, 
                               tRectRetour.w, tRectRetour.h };

    while (paramRunning) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point p = {mouseX, mouseY};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) paramRunning = 0;
            if (e.type == SDL_MOUSEBUTTONDOWN || (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)))) {
                
                if (SDL_PointInRect(&p, &barSon)) {
                    volume = ((p.x - barSon.x) * 100) / barSon.w;
                    if (volume < 0) volume = 0;
                    if (volume > 100) volume = 100;
                    Mix_Volume(-1, (volume * 128) / 100);      
                    Mix_VolumeMusic((volume * 128) / 100);      
                }
                if (SDL_PointInRect(&p, &barLum)) {
                    luminosite = ((p.x - barLum.x) * 255) / barLum.w;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (SDL_PointInRect(&p, &rectSwitchHitbox)) {
                        hitboxes = !hitboxes; 
                    }
                    if (SDL_PointInRect(&p, &rectBtnRetour)) {
                        sauvegarder_parametres();
                        paramRunning = 0; 
                    }
                }
            }
        }

        // --- DESSIN ---
        SDL_SetRenderDrawColor(renderer, 27, 45, 45, 255); 
        SDL_RenderClear(renderer);
        if (texBG) SDL_RenderCopy(renderer, texBG, NULL, NULL);

        // Textes Réglages
        SDL_RenderCopy(renderer, txtTitre, NULL, &posTitre);
        SDL_RenderCopy(renderer, txtSon, NULL, &posSon);
        SDL_RenderCopy(renderer, txtLum, NULL, &posLum);

        // Barres
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderFillRect(renderer, &barSon);
        SDL_RenderFillRect(renderer, &barLum);

        SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); // Vert
        SDL_Rect fillSon = { barSon.x, barSon.y, (volume * barSon.w) / 100, barSon.h };
        SDL_RenderFillRect(renderer, &fillSon);

        SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255); // Bleu
        SDL_Rect fillLum = { barLum.x, barLum.y, (luminosite * barLum.w) / 255, barLum.h };
        SDL_RenderFillRect(renderer, &fillLum);

        // Texte Hitboxes
        SDL_Rect tRectHitTxt;
        SDL_Texture* txtHitTxt = loadText(renderer, font, "AFFICHER HITBOXES", blanc, &tRectHitTxt);
        if (txtHitTxt) {
            SDL_Rect posHitTxtFixed = { rectTxtHitbox.x + (rectTxtHitbox.w - tRectHitTxt.w)/2,
                                         rectTxtHitbox.y + (rectTxtHitbox.h - tRectHitTxt.h)/2,
                                         tRectHitTxt.w, tRectHitTxt.h };
            SDL_RenderCopy(renderer, txtHitTxt, NULL, &posHitTxtFixed);
            SDL_DestroyTexture(txtHitTxt); 
        }

        // Switch Hitbox
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &rectSwitchHitbox);

        if (hitboxes) SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255); 
        else SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255); 
        
        SDL_Rect switchInner = { rectSwitchHitbox.x + 2, rectSwitchHitbox.y + 2, rectSwitchHitbox.w - 4, rectSwitchHitbox.h - 4 };
        SDL_RenderFillRect(renderer, &switchInner);

        int curseurW = rectSwitchHitbox.w / 3; 
        SDL_Rect rectCurseur = { (hitboxes) ? rectSwitchHitbox.x + rectSwitchHitbox.w - curseurW - 2 : rectSwitchHitbox.x + 2,
                                 rectSwitchHitbox.y - 3, curseurW, rectSwitchHitbox.h + 6 };

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
        SDL_RenderFillRect(renderer, &rectCurseur);
        SDL_Rect curseurInner = { rectCurseur.x + 2, rectCurseur.y + 2, rectCurseur.w - 4, rectCurseur.h - 4 };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        SDL_RenderFillRect(renderer, &curseurInner);

        // ==========================================
        // --- DESSIN DES COMMANDES EN COLONNES ---
        // ==========================================
        
        SDL_Rect posCmdTitre = { rightX, 160, tRectCmdTitre.w, tRectCmdTitre.h };
        SDL_RenderCopy(renderer, txtCmdTitre, NULL, &posCmdTitre);

        // Calcul dynamique pour aligner toutes les touches sur le texte le plus long ("Deplacement :")
        int alignX = rightX + tRectMouvDesc.w + 20;

        // --- 1. Déplacement ---
        int cmdY1 = 220; 
        SDL_Rect pMouv = { rightX, cmdY1, tRectMouvDesc.w, tRectMouvDesc.h };
        SDL_RenderCopy(renderer, txtMouvDesc, NULL, &pMouv);
        
        drawKey(renderer, alignX, cmdY1 - 10, txtQ, tR_Q);           
        drawKey(renderer, alignX + 55, cmdY1 - 10, txtD, tR_D);      

        SDL_Rect pOu1 = { alignX + 120, cmdY1, tRectOu.w, tRectOu.h };
        SDL_RenderCopy(renderer, txtOu, NULL, &pOu1);

        drawKey(renderer, alignX + 120 + tRectOu.w + 15, cmdY1 - 10, txtLeft, tR_Left);      
        drawKey(renderer, alignX + 175 + tRectOu.w + 15, cmdY1 - 10, txtRight, tR_Right); 

        // --- 2. Saut --- 
        int cmdY2 = 290; 
        SDL_Rect pSaut = { rightX, cmdY2, tRectSautDesc.w, tRectSautDesc.h };
        SDL_RenderCopy(renderer, txtSautDesc, NULL, &pSaut);

        drawKey(renderer, alignX, cmdY2 - 10, txtZ, tR_Z);
        
        SDL_Rect pOu2 = { alignX + 65, cmdY2, tRectOu.w, tRectOu.h };
        SDL_RenderCopy(renderer, txtOu, NULL, &pOu2);

        drawKey(renderer, alignX + 65 + tRectOu.w + 15, cmdY2 - 10, txtUp, tR_Up);

        // --- 3. Pause --- 
        int cmdY3 = 360; 
        SDL_Rect pPause = { rightX, cmdY3, tRectPauseDesc.w, tRectPauseDesc.h };
        SDL_RenderCopy(renderer, txtPauseDesc, NULL, &pPause);

        drawKey(renderer, alignX, cmdY3 - 10, txtP, tR_P);

        // --- 4. Quitter ---
        int cmdY4 = 430; 
        SDL_Rect pQuit = { rightX, cmdY4, tRectQuitDesc.w, tRectQuitDesc.h };
        SDL_RenderCopy(renderer, txtQuitDesc, NULL, &pQuit);
        
        drawWideKey(renderer, alignX, cmdY4 - 10, 110, txtEchap, tR_Echap);

        // ==========================================


        // Bouton Retour 
        if (texBoutonBois) SDL_RenderCopy(renderer, texBoutonBois, NULL, &rectBtnRetour);
        SDL_RenderCopy(renderer, txtRetour, NULL, &posRetourTxt);
        
        if (SDL_PointInRect(&p, &rectBtnRetour)) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
            SDL_RenderFillRect(renderer, &rectBtnRetour);
        }

        // Luminosité
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - luminosite); 
        SDL_Rect fullScreen = {0, 0, width, height};
        SDL_RenderFillRect(renderer, &fullScreen);

        SDL_RenderPresent(renderer);
    }

    // Nettoyage complet
    SDL_DestroyTexture(txtTitre); SDL_DestroyTexture(txtSon); SDL_DestroyTexture(txtLum); SDL_DestroyTexture(txtRetour);
    SDL_DestroyTexture(txtCmdTitre); SDL_DestroyTexture(txtMouvDesc); SDL_DestroyTexture(txtSautDesc); SDL_DestroyTexture(txtPauseDesc); SDL_DestroyTexture(txtQuitDesc); SDL_DestroyTexture(txtOu);
    SDL_DestroyTexture(txtZ); SDL_DestroyTexture(txtQ); SDL_DestroyTexture(txtD); SDL_DestroyTexture(txtP); SDL_DestroyTexture(txtEchap);
    SDL_DestroyTexture(txtUp); SDL_DestroyTexture(txtLeft); SDL_DestroyTexture(txtRight);
    SDL_DestroyTexture(texBoutonBois);
}


// ==============================================================
// ================= SELECTION DE LA MAP ========================
// ==============================================================

MenuResult afficher_selection_map(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG2) {
    (void)font; 
    int selectionRunning = 1;
    MenuResult result = MENU_NONE;
    SDL_Event e;
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color or_couleur = {255, 215, 0, 255};

    TTF_Font* font_titre = TTF_OpenFont("assets/NSuperMario.ttf", 64);
    TTF_Font* font_boutons = TTF_OpenFont("assets/NSuperMario.ttf", 32);
    SDL_Texture* texBoutonBois = IMG_LoadTexture(renderer, "assets/bouton_bois.png");

    int centerX = width / 2;
    int size = 250; 
    int btnW = 300, btnH = 70;

    // Positions des zones cliquables
    SDL_Rect rectMap1 = { centerX - 440, height - 470, size, size };
    SDL_Rect rectMap2 = { centerX - 90, height - 470, size, size };
    SDL_Rect rectRetour = { centerX - btnW/2, height - 150, btnW, btnH };

    SDL_Rect tRectTitre, tRectRetour;
    SDL_Texture* txtTitre = loadText(renderer, font_titre, "MARIO-BROS-LIKE", or_couleur, &tRectTitre);
    SDL_Texture* txtRetour = loadText(renderer, font_boutons, "RETOUR", blanc, &tRectRetour);
    SDL_Rect posTitre = { centerX - tRectTitre.w/2, height/10, tRectTitre.w, tRectTitre.h };

    while (selectionRunning) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mP = {mx, my};

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_DestroyTexture(txtTitre);
                SDL_DestroyTexture(txtRetour);
                SDL_DestroyTexture(texBoutonBois);
                TTF_CloseFont(font_titre);
                TTF_CloseFont(font_boutons);
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
        if (texBG2) SDL_RenderCopy(renderer, texBG2, NULL, NULL);
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

    SDL_DestroyTexture(txtTitre);
    SDL_DestroyTexture(txtRetour);
    SDL_DestroyTexture(texBoutonBois);
    TTF_CloseFont(font_titre);
    TTF_CloseFont(font_boutons);

    return result;
}