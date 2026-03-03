#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Variables globales
extern int volume;
extern int luminosite;

// Codes de retour du menu
typedef enum {
    MENU_QUIT = 0,
    MENU_PLAY = 1,
    MENU_OPTIONS = 2
} MenuResult;

/**
 * charge les parametres enregistrer
 */
void charger_parametres();

/**
 * sauvegarde les parametres
 */
void sauvegarder_parametres();

/**
 * Affiche le menu et attend une action du joueur.
 * @param renderer Le renderer SDL déjà créé dans le main
 * @param width Largeur réelle de la fenêtre
 * @param height Hauteur réelle de la fenêtre
 * @return L'action choisie (JOUER, QUITTER, etc.)
 */
MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height);


/**
 * Affiche le menu paramètre
 * @param renderer Le renderer SDL déjà créé dans le main
 * @param width Largeur réelle de la fenêtre
 * @param height Hauteur réelle de la fenêtre
 * @param font Police d'écriture
 * @param textBG Image de fond
 */
void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG);

#endif