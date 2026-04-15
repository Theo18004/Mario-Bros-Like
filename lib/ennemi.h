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


/**
 * @struct Podoboo
 * @brief Boule de feu jaillissante.
 */
typedef struct Podoboo {
    SDL_Rect rect;  /**< Position et taille du Podoboo */
    int minY;       /**< Limite haute (plafond) */
    int maxY;       /**< Limite basse (lave) */
    float speedY;   /**< Vitesse de déplacement vertical */
    int vivant;     /**< 1 si actif, 0 si supprimé */
} Podoboo;

/**
 * @struct Coquilas
 * @brief Ennemi rampant patrouilleur.
 */
typedef struct Coquilas {
    SDL_Rect rect;      /**< Hitbox de l'ennemi */
    float velY;         /**< Vélocité verticale */
    float speed;        /**< Vitesse de marche */
    int direction;      /**< 1 pour droite, -1 pour gauche */
    int onGround;       /**< État au sol */
    PlayerState state;  /**< État d'animation */
    int vivant;         /**< 1 si actif, 0 si supprimé */
} Coquilas;

/**
 * @struct Presse
 * @brief Obstacle mortel mécanique.
 */
typedef struct Presse {
    SDL_Rect baseRect;      /**< La partie fixe de la presse (plafond) */
    SDL_Rect extensionRect; /**< La hitbox dynamique qui va grandir et rétrécir */
    int currentFrame;       /**< Frame d'animation actuelle */
    Uint32 frameTimer;      /**< Pour contrôler la vitesse d'animation */
    Uint32 startDelay;      /**< Délai avant de commencer l'animation (désynchronisation) */
    int activated;          /**< 1 si la presse a été activée, 0 sinon */
    int animationState;     /**< 0 = en extension (chute), 1 = en rétractation */
    int vivant;             /**< 1 si actif, 0 si supprimé */
} Presse;

#define NB_PODOBOO 11
#define NB_COQUILAS 3
#define NB_JEAN_CLAUDE 4
#define NB_LOUPAS 5
#define NB_THWOMPS 8
#define NB_OLAF 10  
#define NB_ALIENS 8
#define NB_PRESSES 12
#define NB_HARV 5

/**
 * @brief Initialise les paramètres de loopas.
 * @param e Pointeur vers l'ennemi.
 * @param x, y Coordonnées d'apparition.
 */
void init_loupas(Ennemi* e, int x, int y);

/**
 * @brief Met à jour le comportement de loopas.
 * @param e Pointeur vers l'ennemi.
 * @param map Carte du niveau.
 * @param levelID Identifiant du niveau.
 */
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

/**
 * @brief Initialise les paramètres du thwomp.
 * @param t Pointeur vers le Thwomp.
 * @param x, y Coordonnées d'apparition.
 */
void init_thwomp(Thwomp* t, int x, int y);

/**
 * @brief Met à jour le comportement du Thwomp.
 * @param t Pointeur vers le Thwomp.
 * @param p Pointeur vers le joueur.
 * @param map Carte du niveau.
 * @param levelID Identifiant du niveau.
 */
void update_thwomp(Thwomp* t, Player* p, int* map, int levelID);

/**
 * @brief Affiche le Thwomp à l'écran.
 * @param renderer Le renderer SDL.
 * @param t Pointeur vers le Thwomp.
 * @param scrollX Décalage caméra X.
 * @param scrollY Décalage caméra Y.
 * @param texThwomp La sprite sheet du Thwomp.
 */
void render_thwomp(SDL_Renderer* renderer, Thwomp* t, int scrollX, int scrollY, SDL_Texture* texThwomp);

/**
 * @brief Initialise les paramètres du podoboo.
 * @param p Pointeur vers Podoboo.
 * @param x, y Coordonnées d'apparition.
 * @param limYBas limite de profondeur.
 * @param limYHaut limite de hauteur
 */
void init_podoboo(Podoboo* p, int posX, int limYBas, int limYHaut);

/**
 * @brief Met à jour le comportement de la Podoboo.
 * @param p Pointeur vers la Podoboo.
 * @param cam Pointeur vers la caméra.
 */
void update_podoboo(Podoboo* p, Camera * cam);

/**
 * @brief Affiche la Podoboo à l'écran.
 * @param renderer Le renderer SDL.
 * @param p Pointeur vers la Podoboo.
 * @param camX Décalage caméra X.
 * @param camY Décalage caméra Y.
 * @param texPodoboo La sprite sheet de la Podoboo.
 */
void render_podoboo(SDL_Renderer* renderer, Podoboo* p, int camX, int camY, SDL_Texture* texPodoboo);

/**
 * @brief Initialise les paramètres de coquilas.
 * @param c Pointeur vers Coquilas.
 * @param x, y Coordonnées d'apparition.
 */
void init_coquilas(Coquilas* c, int x, int y); 

/**
 * @brief Met à jour le comportement de Coquilas.
 * @param c Pointeur vers Coquilas.
 * @param map Carte du niveau.
 * @param levelID Identifiant du niveau.
 */
void update_coquilas(Coquilas* c, int* map, int levelID);

/**
 * @brief Affiche les Coquilas à l'écran.
 * @param renderer Le renderer SDL.
 * @param c Pointeur vers Coquilas.
 * @param scrollX Décalage caméra X.
 * @param scrollY Décalage caméra Y.
 * @param texCoquilas La sprite sheet de Coquilas.
 */
void render_coquilas(SDL_Renderer* renderer, Coquilas* c, int scrollX, int scrollY, 
                   SDL_Texture* texCoquilas);
/**
 * @brief Initialise les paramètres de Jean-Claude.
 * @param e Pointeur vers l'ennemi.
 * @param x, y Coordonnées d'apparition.
 */
void init_jc(Ennemi* e, int x, int y);

/**
 * @brief Met à jour le comportement de la Jean-Claude.
 * @param e Pointeur vers Jean-Claude.
 * @param map Carte du niveau.
 * @param levelID Identifiant du niveau.
 */
void update_jc(Ennemi* e, int* map, int levelID);

/**
 * @brief Affiche Jean-Claude à l'écran.
 * @param renderer Le renderer SDL.
 * @param e Pointeur vers Jean-Claude.
 * @param scrollX Décalage caméra X.
 * @param scrollY Décalage caméra Y.
 * @param texMomie La sprite sheet de Jean-Claude.
 */
void render_jc(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, SDL_Texture* texMomie);

/**
 * @brief Initialise les paramètres de Olaf.
 * @param e Pointeur vers l'ennemi.
 * @param x, y Coordonnées d'apparition.
 */
void init_snowman(Ennemi* e, int x, int y);

/**
 * @brief Met à jour le comportement de Olaf.
 * @param e Pointeur vers Olaf.
 * @param map Carte du niveau.
 * @param levelID Identifiant du niveau.
 */
void update_snowman(Ennemi* e, int* map, int levelID);

/**
 * @brief Affiche Olaf à l'écran.
 * @param renderer Le renderer SDL.
 * @param e Pointeur vers Olaf.
 * @param scrollX Décalage caméra X.
 * @param scrollY Décalage caméra Y.
 * @param texOlaf La sprite sheet d'Olaf.
 */
void render_snowman(SDL_Renderer* renderer, Ennemi* e, int scrollX, int scrollY, SDL_Texture* texOlaf);

/**
 * @brief Initialise les paramètres de la Presse.
 * @param p Pointeur vers Presse.
 * @param x, y Coordonnées d'apparition.
 * @param offset Décalage de la presse.
 */
void init_presse(Presse* p, int x, int y, Uint32 offset);

/**
 * @brief Met à jour le comportement de la Presse.
 * @param p Pointeur vers la Presse.
 */
void update_presse(Presse* p);

/**
 * @brief Affiche la Presse à l'écran.
 * @param renderer Le renderer SDL.
 * @param p Pointeur vers la Presse.
 * @param camX Décalage caméra X.
 * @param camY Décalage caméra Y.
 * @param tex La sprite sheet de la Presse.
 */
void render_presse(SDL_Renderer* renderer, Presse* p, int camX, int camY, SDL_Texture* tex);

/**
 * @brief Initialise les paramètres de l'Alien.
 * @param h Pointeur vers l'ennemi.
 * @param x, y Coordonnées d'apparition.
 */
void init_alien(Ennemi* a, int x, int y);

/**
 * @brief Met à jour le comportement de l'Alien.
 * @param a Pointeur vers l'Alien.
 * @param map Carte du niveau.
 * @param levelID Identifiant du niveau.
 */
void update_alien(Ennemi* a, int* map, int levelID);

/**
 * @brief Affiche l'Alien à l'écran.
 * @param renderer Le renderer SDL.
 * @param a Pointeur vers l'Alien.
 * @param scrollX Décalage caméra X.
 * @param scrollY Décalage caméra Y.
 * @param texAlien La sprite sheet de l'Alien.
 */
void render_alien(SDL_Renderer* renderer, Ennemi* a, int scrollX, int scrollY, SDL_Texture* texAlien);

/**
 * @brief Initialise les paramètres de la voiture Harv.
 * @param h Pointeur vers l'ennemi.
 * @param x, y Coordonnées d'apparition.
 * @param speed Vitesse de la voiture.
 */
void init_harv(Ennemi* h, int x, int y, float speed);

/**
 * @brief Met à jour le comportement de la voiture.
 * @param h Pointeur vers la voiture.
 * @param map Carte du niveau.
 * @param levelID Identifiant du niveau.
 */
void update_harv(Ennemi* h, int* map, int levelID);

/**
 * @brief Affiche la voiture.
 * @param renderer Le renderer SDL.
 * @param h Pointeur vers la voiture.
 * @param scrollX, scrollY Décalage caméra.
 * @param texHarv Texture de la voiture.
 */
void render_harv(SDL_Renderer* renderer, Ennemi* h, int scrollX, int scrollY, SDL_Texture* texHarv);

#endif