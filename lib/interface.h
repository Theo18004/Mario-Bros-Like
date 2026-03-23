/**
 * @file interface.h
 * @brief Affichage des éléments du HUD (vies, progression).
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Items.h"

struct Player;
typedef struct Player Player;

typedef struct {
    int value;    /**< Valeur du score actuel */
    int bonus;   /**< Bonus accumulé (Pièces / Monstres) */
    int max_x;    /**< Distance maximum atteinte par le joueur */
    SDL_Rect rect; /**< Position d'affichage du score */
} Score;

/**
 * @brief Affiche les cœurs représentant les vies restantes.
 * @param lives Nombre de vies à afficher.
 */
 void render_lives(SDL_Renderer* renderer, SDL_Texture** texVies, int lives);

/**
 * @brief Affiche les pièces ramassées dans le HUD.
 * @param texPiecesHUD Tableau de textures pour les différentes étapes d'affichage des pièces.
 * @param mesPieces Tableau des pièces présentes dans le niveau.
 */
void render_pieces_hud(SDL_Renderer* renderer, SDL_Texture** texPiecesHUD, Piece* mesPieces);

/**
 * @brief Affiche un timer à l'écran.
 * @param font Police utilisée pour afficher le texte du timer.
 * @param tempsRestant Temps restant en secondes à afficher.
 */
void render_timer(SDL_Renderer* renderer, TTF_Font* font, int tempsRestant);

/**
 * @brief Affiche le score du joueur à l'écran.
 * @param font Police utilisée pour afficher le texte du score.
 * @param s Pointeur vers la structure du score à afficher.
 */
void render_score(SDL_Renderer* renderer, TTF_Font* font, Score* s);


void init_score(Score* s);
void update_score(Score* s, int player_x);
void reset_score(Score* s);


/**
 * @brief Affiche un écran Game Over lorsque le joueur n'a plus de vies.
 * @param font Police utilisée pour afficher le texte.
 * @param player Pointeur vers la structure du joueur.
 * @param score Pointeur vers la structure du score.
 * @param score_affichage_fin Score affiché à la fin du jeu.
 * @param meilleur_score Meilleur score atteint.
 */
int gameover(SDL_Renderer* renderer, TTF_Font* font, Player* player, int score_affichage_fin, int meilleur_score);


/**
 * @brief Affiche l'écran de victoire avec les statistiques de la partie.
 * @param renderer Le renderer SDL.
 * @param font La police pour le texte.
 * @param player Pointeur vers le joueur (pour les vies).
 * @param score Pointeur vers la structure du score.
 * @param temps_restant Le temps restant pour finir le niveau.
 * @param etoiles Le nombre de pièces/étoiles récoltées.
 * @return 0 pour quitter, 1 pour retourner au menu principal (ou niveau suivant).
 */
int victory_screen(SDL_Renderer* renderer, TTF_Font* font, Player* player, Score* score, int temps_restant, int etoiles);
#endif
