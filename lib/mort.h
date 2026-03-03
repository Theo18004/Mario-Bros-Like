#ifndef MORT_H
#define MORT_H

#include "player.h"

// Vérifie toutes les conditions qui peuvent tuer le joueur ( hors map / enemis)
int verifier_conditions_mort(Player* p, int mapPixelHeight);

// Gère l'action de mourir ( animation / respawn)
void gerer_mort_joueur(Player* p, int spawnX, int spawnY);

#endif