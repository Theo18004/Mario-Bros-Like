/**
 * @file ennemi.h
 * @brief Gestion des entités ennemies (Loup).
 */

#ifndef ENNEMI_H
#define ENNEMI_H

#include <SDL.h>
#include "player.h"
#include "camera.h"

extern Mix_Chunk * bouleFeu;

/**
 * @enum ThwompState
 * @brief Les différents états du Thwomp.
 */
typedef enum {
    THWOMP_IDLE,     /**< Attend en hauteur */
    THWOMP_FALLING,  /**< Tombe rapidement vers le sol */
    THWOMP_GROUNDED, /**< Écrasé au sol (en pause) */
    THWOMP_RISING    /**< Remonte lentement à sa position initiale */
} ThwompState;

/**
 * @struct Ennemi
 * @brief Données d'un ennemi Loup patrouilleur.
 */
typedef struct Ennemi {
    SDL_Rect rect;      /**< Hitbox de l'ennemi */
    float velY;         /**< Vélocité verticale */
    float speed;        /**< Vitesse de marche */
    int direction;      /**< 1 pour droite, -1 pour gauche */
    int onGround;       /**< État au sol */
    PlayerState state;  /**< État d'animation */
    int vivant;         /**< 1 si actif, 0 si supprimé */
} Ennemi;

/**
 * @struct Thwomp
 * @brief Données de l'ennemi Thwomp.
 */
typedef struct Thwomp {
    SDL_Rect rect;       /**< Hitbox du Thwomp */
    int startY;          /**< Hauteur d'origine (plafond) */
    float velY;          /**< Vélocité de chute */
    ThwompState state;   /**< État actuel */
    Uint32 groundTimer;  /**< Chronomètre pour le temps d'arrêt au sol */
    int vivant;          /**< 1 si actif, 0 si supprimé */
} Thwomp;


typedef struct Podoboo {
    SDL_Rect rect;  /**< Position et taille du Podoboo */
    int minY;       /**< Limite haute */
    int maxY;       /**< Limite basse */
    float speedY;   /**< Vitesse de déplacement vertical */
    int vivant;     /**< 1 si actif, 0 si supprimé */
} Podoboo;


typedef struct Coquilas {
    SDL_Rect rect;      /**< Hitbox de l'ennemi */
    float velY;         /**< Vélocité verticale */
    float speed;        /**< Vitesse de marche */
    int direction;      /**< 1 pour droite, -1 pour gauche */
    int onGround;       /**< État au sol */
    PlayerState state;  /**< État d'animation */
    int vivant;         /**< 1 si actif, 0 si supprimé */
} Coquilas;

#define NB_PODOBOO 11
#define NB_COQUILAS 3
#define NB_JEAN_CLAUDE 4
#define NB_LOUPAS 5
#define NB_THWOMPS 8
#define NB_OLAF 2

void init_loupas(Ennemi* e, int x, int y);
void update_loupas(Ennemi* e, int* map, int levelID);
/**
 * @brief Affiche l'ennemi Loup en animant sa Sprite Sheet.
 * @param renderer Le renderer SDL.
 * @param e Pointeur vers l'ennemi.
 * @param scrollX Décalage caméra X.
 * @param scrollY Décalage caméra Y.
 * @param texEnnemi La sprite sheet du loup (wolfsheet.png).
 */
void render_loupas(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, 
                   SDL_Texture* texEnnemi);

void init_thwomp(Thwomp* t, int x, int y);
void update_thwomp(Thwomp* t, Player* p, int* map, int levelID);
void render_thwomp(SDL_Renderer* renderer, Thwomp* t, int scrollX, int scrollY, SDL_Texture* texThwomp);

void init_podoboo(Podoboo* p, int posX, int limYBas, int limYHaut);
void update_podoboo(Podoboo* p, Camera * cam);
void render_podoboo(SDL_Renderer* renderer, Podoboo* p, int camX, int camY, SDL_Texture* texPodoboo);


void init_coquilas(Coquilas* c, int x, int y); 
void update_coquilas(Coquilas* c, int* map, int levelID);
void render_coquilas(SDL_Renderer* renderer, Coquilas* c, int scrollX, int scrollY, 
                   SDL_Texture* texCoquilas);


void init_jc(Ennemi* e, int x, int y);
void update_jc(Ennemi* e, int* map, int levelID);
void render_jc(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, SDL_Texture* texMomie);

void init_snowman(Ennemi* e, int x, int y);
void update_snowman(Ennemi* e, int* map, int levelID);
void render_snowman(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, SDL_Texture* texOlaf);

#endif