/**
 * @file interface.h
 * @brief Affichage des éléments du HUD (vies, progression, menu pause).
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Items.h"


struct Player;
typedef struct Player Player;

struct Ennemi;
typedef struct Ennemi Ennemi;

struct Thwomp;
typedef struct Thwomp Thwomp;

struct Podoboo;
typedef struct Podoboo Podoboo;

struct Coquilas;
typedef struct Coquilas Coquilas;

struct Flag;
typedef struct Flag Flag;

struct Presse;
typedef struct Presse Presse;

typedef struct {
    int value;    /**< Valeur du score actuel */
    int bonus;   /**< Bonus accumulé (Pièces / Monstres) */
    int max_x;    /**< Distance maximum atteinte par le joueur */
    SDL_Rect rect; /**< Position d'affichage du score */
} Score;

// --- STRUCTURE DU MENU PAUSE ---
typedef struct {
    SDL_Texture* texIcons;
    SDL_Rect srcPauseIcon;
    SDL_Rect srcPlay;
    SDL_Rect srcRestart;
    SDL_Rect srcParam;
    SDL_Rect srcHome;
    SDL_Rect dstPausePos;
    SDL_Rect dstPlay;
    SDL_Rect dstRestart;
    SDL_Rect dstParam;
    SDL_Rect dstHome;
} PauseMenu;

/**
 * @brief Initialise les coordonnées et textures du menu pause.
 */
void init_pause_menu(PauseMenu* pm, SDL_Texture* texIcons, int logicalW, int logicalH);

/**
 * @brief Affiche le menu pause (ou juste l'engrenage si pas en pause).
 */
void render_pause_menu(SDL_Renderer* renderer, PauseMenu* pm, int enPause, int logicalW, int logicalH);

/**
 * @brief Affiche les cœurs représentant les vies restantes.
 */
 void render_lives(SDL_Renderer* renderer, SDL_Texture** texVies, int lives);

/**
 * @brief Affiche les pièces ramassées dans le HUD.
 */
void render_pieces_hud(SDL_Renderer* renderer, SDL_Texture** texPiecesHUD, Piece* mesPieces);

/**
 * @brief Affiche un timer à l'écran.
 */
void render_timer(SDL_Renderer* renderer, TTF_Font* font, int tempsRestant);

void render_debug_hitboxes(SDL_Renderer* renderer, Player* player, 
                           Ennemi* loupas, Thwomp* thwomps, Podoboo* podoboo, 
                           Coquilas* coquilas, Ennemi* jc, Ennemi* olaf, Ennemi* aliens, Presse* presses, Ennemi* harvs, Flag* drapeau, 
                           int camX, int camY) ;

/**
 * @brief Affiche le score du joueur à l'écran.
 */
void render_score(SDL_Renderer* renderer, TTF_Font* font, Score* s);


void init_score(Score* s);
void update_score(Score* s, int player_x);
void reset_score(Score* s);

/**
 * @brief Affiche un écran Game Over.
 */
int gameover(SDL_Renderer* renderer, TTF_Font* font, Player* player, int score_affichage_fin, int meilleur_score);

/**
 * @brief Affiche l'écran de victoire.
 */
int victory_screen(SDL_Renderer* renderer, TTF_Font* font, Player* player, Score* score, int temps_restant, int etoiles);

#endif