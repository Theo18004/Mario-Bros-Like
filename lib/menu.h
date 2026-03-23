/**
 * @file menu.h
 * @brief Système de menus (principal, options) et gestion des paramètres.
 */

#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern int volume;      /**< Volume global du jeu */
extern int luminosite;  /**< Luminosité de l'écran */

/** @enum MenuResult */
typedef enum {
    MENU_QUIT = 0, /**< L'utilisateur veut quitter */
    MENU_PLAY = 1, /**< L'utilisateur veut lancer la partie */
    MENU_OPTIONS = 2 /**< L'utilisateur est dans les options */
} MenuResult;

void charger_parametres();
void sauvegarder_parametres();
MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height);
void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG);

#endif