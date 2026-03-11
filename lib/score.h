/**
 * @file score.h
 * @brief Système de score basé sur la distance parcourue.
 */

#ifndef SCORE_H
#define SCORE_H

#include <SDL2/SDL.h>

/**
 * @struct Score
 * @brief Stocke le score actuel et le record de distance.
 */
typedef struct {
    int value;    /**< Valeur du score actuel */
    int bonus;   /**< Bonus accumulé (Pièces / Monstres) */
    int max_x;    /**< Distance maximum atteinte par le joueur */
    SDL_Rect rect; /**< Position d'affichage du score */
} Score;

void init_score(Score* s, int x, int y);
void update_score(Score* s, int player_x);
void reset_score(Score* s);
void render_score(SDL_Renderer* renderer, Score* s);

#endif