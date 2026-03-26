/**
 * @file camera.h
 * @brief Gestion de la caméra de défilement (scrolling).
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "player.h"

/**
 * @struct Camera
 * @brief Définit la vue logique de la fenêtre sur le monde.
 */
typedef struct {
    int x; /**< Position horizontale de la caméra */
    int y; /**< Position verticale de la caméra */
    int w; /**< Largeur logique de l'écran */
    int h; /**< Hauteur logique de l'écran */
} Camera;

/**
 * @brief Calcule la position de la caméra pour qu'elle suive le joueur.
 * @param cam Pointeur vers la caméra.
 * @param p Pointeur vers le joueur à suivre.
 * @param mapWidthPixel Largeur totale de la carte en pixels.
 * @param mapHeightPixel Hauteur totale de la carte en pixels.
 */
void update_camera(Camera* cam, Player* p, int mapWidthPixel, int mapHeightPixel);

#endif