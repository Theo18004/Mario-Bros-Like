/**
 * @file interface.h
 * @brief Affichage des éléments du HUD (vies, progression).
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>

/**
 * @brief Affiche les cœurs représentant les vies restantes.
 * @param lives Nombre de vies à afficher.
 */
void render_lives(SDL_Renderer* renderer, SDL_Texture* texHeart, int lives);

/**
 * @brief Affiche la barre de progression du niveau.
 * @param playerX Position X actuelle du joueur.
 * @param mapPixelWidth Largeur totale de la carte en pixels.
 */
void render_progress_bar(SDL_Renderer* renderer, int playerX, int mapPixelWidth);

#endif