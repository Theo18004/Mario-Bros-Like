/**
 * @file menu.h
 * @brief Système de menus (principal, options) et gestion des paramètres.
 */

#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern int volume;      /**< Volume global du jeu (0 à 100) */
extern int luminosite;  /**< Luminosité de l'écran (0 à 255) */
extern int hitboxes;    /**< Affichage des hitboxes de debug (0 = OFF, 1 = ON) */

/** * @enum MenuResult 
 * @brief Identifie l'action sélectionnée par l'utilisateur dans les menus.
 */
typedef enum {
    MENU_QUIT = 0,        /**< L'utilisateur veut quitter */
    MENU_PLAY = 1,        /**< L'utilisateur veut lancer la partie */
    MENU_OPTIONS = 2,     /**< L'utilisateur est dans les options */
    MENU_CHOIX_MAP1 = 3,  /**< L'utilisateur a choisi la map 1 */
    MENU_CHOIX_MAP2 = 4,  /**< L'utilisateur a choisi la map 2 */
    MENU_RESTART = 5,     /**< L'utilisateur veut recommencer */
    MENU_NONE = -1        /**< Aucun choix, retour au menu précédent */
} MenuResult;

/** @brief Charge les paramètres (Volume, Luminosité, Hitboxes) depuis config.txt. */
void charger_parametres();

/** @brief Sauvegarde les paramètres actuels dans config.txt. */
void sauvegarder_parametres();

/**
 * @brief Affiche le menu principal du jeu.
 * @param renderer Le renderer SDL.
 * @param width, height Dimensions physiques de la fenêtre.
 * @return L'action choisie par le joueur (Jouer, Quitter...).
 */
MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height);

/**
 * @brief Affiche le menu des paramètres (Son, Luminosité, Touches).
 * @param renderer Le renderer SDL.
 * @param width, height Dimensions de la fenêtre.
 * @param font Police utilisée pour le texte.
 * @param texBG Texture d'arrière-plan.
 */
void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG);

/**
 * @brief Affiche l'écran de sélection de la carte.
 * @param renderer Le renderer SDL.
 * @param width, height Dimensions de la fenêtre.
 * @param font Police utilisée pour le texte.
 * @param texBG Texture d'arrière-plan.
 * @return Le choix de la map (MENU_CHOIX_MAP1 ou MENU_CHOIX_MAP2).
 */
MenuResult afficher_selection_map(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG);

/**
 * @brief Affiche le menu de pause en cours de partie.
 * @param renderer Le renderer SDL.
 * @param width, height Dimensions de la fenêtre.
 * @param texBoutons Texture des boutons du menu de pause.
 * @return L'action sélectionnée par le joueur.
 */
MenuResult afficher_pause(SDL_Renderer* renderer, int width, int height, SDL_Texture* texBoutons);

#endif