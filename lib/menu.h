#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Codes de retour du menu
typedef enum {
    MENU_QUIT = 0,
    MENU_PLAY = 1,
    MENU_OPTIONS = 2
} MenuResult;

/**
 * Affiche le menu et attend une action du joueur.
 * @param renderer Le renderer SDL déjà créé dans le main
 * @param width Largeur réelle de la fenêtre
 * @param height Hauteur réelle de la fenêtre
 * @return L'action choisie (JOUER, QUITTER, etc.)
 */
MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height);

#endif