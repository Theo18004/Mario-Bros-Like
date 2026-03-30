/**
 * @file mort.h
 * @brief Gestion de la mort du joueur et du respawn.
 */

#ifndef MORT_H
#define MORT_H

#include "player.h"
#include "ennemi.h"
#include "Items.h"
#include "interface.h"
#include "camera.h"


/**
 * @brief Vérifie si le joueur tombe hors de la carte.
 * @return 1 si le joueur a fini son animation de mort et doit réapparaître.
 */
int verifier_conditions_mort(Player* p, int mapPixelHeight);

/**
 * @brief Déclenche la mort du joueur et réinitialise sa position.
 */
void gerer_mort_joueur(Player* p, int spawnX, int spawnY, Score* s);

/**
 * @brief Réinitialise l'intégralité du niveau (joueur, monstres, pièces, caméra).
    * @param total_reset Si 1, réinitialise aussi les vies et le score.
 */
void reset_level(Player* p, Ennemi* loup, Thwomp* thwomps, Podoboo* mesPodoboo, Coquilas* mesCoquilas, Ennemi* jc, Ennemi* mesOlaf, Piece* pieces, Score* s, Camera* cam, int total_reset , int levelID);

#endif
