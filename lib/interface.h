/**
 * @file interface.h
 * @brief Affichage des éléments du HUD (vies, progression).
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "player.h"
#include "score.h"


/**
 * @brief Affiche les cœurs représentant les vies restantes.
 * @param lives Nombre de vies à afficher.
 */
 void render_lives(SDL_Renderer* renderer, SDL_Texture** texVies, int lives);

/**
 * @brief Affiche la barre de progression du niveau.
 * @param playerX Position X actuelle du joueur.
 * @param mapPixelWidth Largeur totale de la carte en pixels.
 */
void render_progress_bar(SDL_Renderer* renderer, int playerX, int mapPixelWidth);

/**
 * @brief Affiche un écran Game Over lorsque le joueur n'a plus de vies.
 * @param font Police utilisée pour afficher le texte.
 * @param player Pointeur vers la structure du joueur.
 * @param score Pointeur vers la structure du score.
 * @param score_affichage_fin Score affiché à la fin du jeu.
 * @param meilleur_score Meilleur score atteint.
 */
int gameover(SDL_Renderer* renderer, TTF_Font* font, Player* player, int score_affichage_fin, int meilleur_score);
#endif
