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
 * @param p Pointeur vers le joueur.
 * @param mapPixelHeight Hauteur de la carte en pixels.
 * @return 1 si le joueur a fini son animation de mort et doit réapparaître.
 */
int verifier_conditions_mort(Player* p, int mapPixelHeight);

/**
 * @brief Déclenche la mort du joueur et réinitialise sa position.
 * @param p Pointeur vers le joueur.
 * @param spawnX Position X de réapparition.
 * @param spawnY Position Y de réapparition.
 * @param s Pointeur vers la structure Score.
 */
void gerer_mort_joueur(Player* p, int spawnX, int spawnY, Score* s);

/**
 * @brief Réinitialise l'intégralité du niveau (joueur, monstres, pièces, caméra).
 * @param p Pointeur vers le joueur.
 * @param mesLoupas Pointeur vers les ennemis de type Loupas.
 * @param thwomps Pointeur vers les ennemis de type Thwomp.
 * @param mesPodoboo Pointeur vers les ennemis de type Podoboo.
 * @param mesCoquilas Pointeur vers les ennemis de type Coquilas.
 * @param jc Pointeur vers l'ennemi de type JC.
 * @param mesOlaf Pointeur vers les ennemis de type Olaf.
 * @param mesAliens Pointeur vers les ennemis de type Aliens.
 * @param mesHarvs Pointeur vers les ennemis de type Harvs.
 * @param presse Pointeur vers les éléments de type Presse.
 * @param pieces Pointeur vers les pièces du niveau.
 * @param s Pointeur vers la structure Score.
 * @param total_reset Si 1, réinitialise aussi les vies et le score.
 */
void reset_level(Player* p, Ennemi* mesLoupas, Thwomp* thwomps, Podoboo* mesPodoboo, 
                 Coquilas* mesCoquilas, Ennemi* jc, Ennemi* mesOlaf, Ennemi* mesAliens, 
                 Ennemi* mesHarvs, Presse* presse, Piece* pieces, Score* s, 
                 Camera* cam, int total_reset, int levelID);

#endif
