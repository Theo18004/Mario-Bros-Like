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
    SDL_Texture* texIcons; /**< Texture des icônes du menu pause */
    SDL_Rect srcPauseIcon;  /**< Source du bouton pause */
    SDL_Rect srcPlay;       /**< Source du bouton play */
    SDL_Rect srcRestart;   /**< Source du bouton restart */
    SDL_Rect srcParam;     /**< Source du bouton paramètres */
    SDL_Rect srcHome;      /**< Source du bouton home */
    SDL_Rect dstPausePos;  /**< Position de destination du bouton pause */
    SDL_Rect dstPlay;      /**< Position de destination du bouton play */
    SDL_Rect dstRestart;   /**< Position de destination du bouton restart */
    SDL_Rect dstParam;     /**< Position de destination du bouton paramètres */
    SDL_Rect dstHome;      /**< Position de destination du bouton home */
} PauseMenu;

/**
 * @brief Initialise les coordonnées et textures du menu pause.
 * @param pm Pointeur vers la structure PauseMenu à initialiser.
 * @param texIcons Texture des icônes du menu pause.
 * @param logicalW Largeur logique de l'écran.
 * @param logicalH Hauteur logique de l'écran.
 */
void init_pause_menu(PauseMenu* pm, SDL_Texture* texIcons, int logicalW, int logicalH);

/**
 * @brief Affiche le menu pause (ou juste l'engrenage si pas en pause).
 * @param renderer Le rendu SDL.
 * @param pm Pointeur vers la structure PauseMenu.
 * @param enPause Indique si le jeu est en pause.
 * @param logicalW Largeur logique de l'écran.
 * @param logicalH Hauteur logique de l'écran.
 */
void render_pause_menu(SDL_Renderer* renderer, PauseMenu* pm, int enPause, int logicalW, int logicalH);

/**
 * @brief Affiche les cœurs représentant les vies restantes.
 * @param renderer Le rendu SDL.
 * @param texVies Les textures des cœurs.
 * @param lives Le nombre de vies restantes.
 */
 void render_lives(SDL_Renderer* renderer, SDL_Texture** texVies, int lives);

/**
 * @brief Affiche les pièces ramassées dans le HUD.
 * @param renderer Le rendu SDL.
 * @param texPiecesHUD Les textures des pièces.
 * @param mesPieces Les informations sur les pièces ramassées.
 */
void render_pieces_hud(SDL_Renderer* renderer, SDL_Texture** texPiecesHUD, Piece* mesPieces);

/**
 * @brief Affiche un timer à l'écran.
 * @param renderer Le rendu SDL.
 * @param font La police à utiliser pour le texte.
 * @param tempsRestant Le temps restant à afficher.
 */
void render_timer(SDL_Renderer* renderer, TTF_Font* font, int tempsRestant);

/**
 * @brief Affiche les hitboxes pour le débogage.
 * @param renderer Le rendu SDL.
 * @param player Pointeur vers le joueur.
 * @param loupas Pointeur vers l'ennemi loupas.
 * @param thwomps Pointeur vers l'ennemi thwomp.
 * @param podoboo Pointeur vers l'ennemi podoboo.
 * @param coquilas Pointeur vers l'ennemi coquilas.
 * @param jc Pointeur vers l'ennemi jc.
 * @param olaf Pointeur vers l'ennemi olaf.
 * @param aliens Pointeur vers l'ennemi aliens.
 * @param presses Pointeur vers l'ennemi presses.
 * @param harvs Pointeur vers l'ennemi harvs.
 * @param drapeau Pointeur vers le drapeau.
 */
void render_debug_hitboxes(SDL_Renderer* renderer, Player* player,
                           Ennemi* loupas, Thwomp* thwomps, Podoboo* podoboo,
                           Coquilas* coquilas, Ennemi* jc, Ennemi* olaf, Ennemi* aliens, Presse* presses, Ennemi* harvs, Flag* drapeau,
                           int camX, int camY) ;

/**
 * @brief Affiche le score du joueur à l'écran.
 * @param renderer Le rendu SDL.
 * @param font La police à utiliser pour le texte.
 * @param s Pointeur vers la structure Score contenant les informations de score.
 */
void render_score(SDL_Renderer* renderer, TTF_Font* font, Score* s);

/**
 * @brief Initialise la structure Score.
 * @param s Pointeur vers la structure Score à initialiser.
 */
void init_score(Score* s);

/**
 * @brief Met à jour le score du joueur.
 * @param s Pointeur vers la structure Score à mettre à jour.
 * @param player_x La position X du joueur.
 */
void update_score(Score* s, int player_x);

/**
 * @brief Réinitialise le score du joueur.
 * @param s Pointeur vers la structure Score à réinitialiser.
 */
void reset_score(Score* s);

/**
 * @brief Affiche un écran Game Over.
 * @param renderer Le rendu SDL.
 * @param font La police à utiliser pour le texte.
 * @param player Pointeur vers le joueur.
 * @param score_affichage_fin Le score à afficher à la fin.
 * @param meilleur_score Le meilleur score à afficher.
 */
int gameover(SDL_Renderer* renderer, TTF_Font* font, Player* player, int score_affichage_fin, int meilleur_score);

/**
 * @brief Affiche l'écran de victoire.
 * @param renderer Le rendu SDL.
 * @param font La police à utiliser pour le texte.
 * @param player Pointeur vers le joueur.
 * @param score Pointeur vers la structure Score contenant les informations de score.
 * @param temps_restant Le temps restant à afficher.
 * @param etoiles Le nombre d'étoiles à afficher.
 */
int victory_screen(SDL_Renderer* renderer, TTF_Font* font, Player* player, Score* score, int temps_restant, int etoiles);

#endif