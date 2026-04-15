/**
 * @file level.h
 * @brief Chargements et gestions des niveaux, des décors et entités.
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "ennemi.h"
#include "Items.h"
#include "flag.h"
#include "defs.h"
#include "player.h"

/**
 * @struct Level
 * @brief Contient toutes les données d'un niveau.
 */
typedef struct {
    int id;                   /**< Identifiant du niveau (ex: 1, 2) */
    int* tileMap;             /**< Tableau 1D représentant la grille des tuiles */
    Tileset tileset;          /**< Texture et dimensions du tileset utilisé */
    SDL_Texture* bgs[15];     /**< Textures des différents plans de fond et décors */
    SDL_Rect bgsPos[15];      /**< Positions des décors fixes */
    SDL_Point playerStart;    /**< Coordonnées de départ du joueur */
    int mapPixelWidth;        /**< Largeur totale du niveau en pixels */
    int mapPixelHeight;       /**< Hauteur totale du niveau en pixels */
} Level;

// Fonctions de gestion de map

/**
 * @brief Alloue et charge un niveau en mémoire.
 * @param renderer Le renderer SDL.
 * @param levelID Identifiant du niveau à charger.
 * @return Pointeur vers la structure Level chargée.
 */
Level* load_level(SDL_Renderer* renderer, int levelID);

/**
 * @brief Alloue et charge le niveau mémoire.
 * @param lvl le niveau actuel.
 * @param loupas, thwomps, podoboo, coquilas, jc, olaf, presse, mesAliens, mesHarvs Tableaux des différents ennemis.
 * @param pieces Tableaux des pièces.
 * @param checkpoint Tableaux des points de sauvegardes.
 * @param flag Pointeur vers le drapeau de fin.
 */
void spawn_level_entities(Level* lvl, Ennemi* loupas, Thwomp* thwomps, Podoboo* podoboo, Coquilas* coquilas, Ennemi* jc, Ennemi* olaf, Presse* presse, Ennemi* mesAliens, Ennemi* mesHarvs, Piece* pieces, Checkpoint* checkpoints, Flag* flag);

/**
 * @brief Dessine les fonds du niveau avec un effet de parallaxe.
 * @param renderer Le renderer SDL.
 * @param lvl Pointeur vers le niveau.
 * @param cameraX Décalage X de la caméra.
 * @param cameraY Décalage Y de la caméra.
 * @param screenW Largeur de l'écran logique.
 * @param screenH Hauteur de l'écran logique.
 */
void draw_level_backgrounds(SDL_Renderer* renderer, Level* lvl, int cameraX, int cameraY, int screenW, int screenH);

/**
 * @brief Libère la mémoire allouée au niveau.
 * @param lvl Pointeur vers le niveau.
 */
void cleanup_level(Level* lvl);

#endif