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
extern int hitboxes; // 0 = OFF, 1 = ON

/** @enum MenuResult */
typedef enum {
    MENU_QUIT = 0, /**< L'utilisateur veut quitter */
    MENU_PLAY = 1, /**< L'utilisateur veut lancer la partie */
    MENU_OPTIONS = 2, /**< L'utilisateur est dans les options */
    MENU_CHOIX_MAP1 = 3, /**< L'utilisateur a choisi la map 1 */
    MENU_CHOIX_MAP2 = 4,  /**< L'utilisateur a choisi la map 2 */
    MENU_RESTART = 5,
    MENU_NONE = -1 /**< Aucun choix retour au menu précédent */
} MenuResult;

void charger_parametres();
void sauvegarder_parametres();
MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height);
void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG);
MenuResult afficher_selection_map(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG);
MenuResult afficher_pause(SDL_Renderer* renderer, int width, int height, SDL_Texture* texBoutons);

#endif