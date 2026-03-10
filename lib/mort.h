/**
 * @file mort.h
 * @brief Gestion de la mort du joueur et du respawn.
 */

#ifndef MORT_H
#define MORT_H

#include "player.h"
#include "score.h"

/**
 * @brief Vérifie si le joueur tombe hors de la carte.
 * @return 1 si le joueur a fini son animation de mort et doit réapparaître.
 */
int verifier_conditions_mort(Player* p, int mapPixelHeight);

/**
 * @brief Déclenche la mort du joueur et réinitialise sa position.
 */
void gerer_mort_joueur(Player* p, int spawnX, int spawnY, Score* s);

#endif