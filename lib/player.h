#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

// Physique
#define GRAVITY 0.5f
#define JUMP_FORCE -12.0f
#define GROUND_Y 540

// Animation
typedef enum {
    ANIM_IDLE,
    ANIM_RUN,
    ANIM_JUMP
} AnimState;

/**
 * @struct Player
 * @brief Structure regroupant toutes les données du joueur.
 * * Cette structure gère à la fois la partie physique (hitbox, vitesse) 
 * et la partie visuelle (textures, états d'animation).
 */

typedef struct {
    SDL_Rect rect;      /**< Hitbox utilisée pour les collisions */
    float velocityY;    /**< Vitesse verticale pour la gestion de la gravité */
    int isJumping;      /**< État du saut : 1 si le joueur est en l'air, 0 sinon */
    
    SDL_Texture* texIdle; /**< Sprite sheet pour l'état immobile */
    SDL_Texture* texRun;  /**< Sprite sheet pour la course */
    SDL_Texture* texJump; /**< Sprite sheet pour le saut */
    
    AnimState animState; /**< État actuel de l'animation (IDLE, RUN, JUMP) */
    int facingLeft;      /**< Direction du regard : 1 pour gauche, 0 pour droite */
    int frameIndex;      /**< Index de la frame d'animation actuelle */
    float frameTimer;    /**< Compteur pour le changement de frame */
} Player;

/**
 * @brief Met à jour la position, la physique et l'animation du joueur à chaque frame.
 * @param player Pointeur vers la structure du joueur à mettre à jour.
 * @note Gère les entrées clavier, applique la gravité et change l'état d'animation.
 */
void updatePlayer(Player* player);

/**
 * @brief Gère les déplacements horizontaux.
 * @param player Pointeur vers le joueur.
 * @param speed Vitesse de déplacement en pixels.
 */
void avancer(Player* player, int speed);

/**
 * @brief Déclenche un saut si le joueur est au sol.
 * @param player Pointeur vers le joueur.
 */
void saut(Player* player);

/**
 * @brief Permet au joueur de descendre plus rapidement.
 * * Ajoute une force de gravité supplémentaire lorsque la touche flèche bas est pressée.
 * @param player Pointeur vers la structure du joueur.
 */
void descendre(Player* player);

#endif