#include "collision.h"
#include "map.h"

// Fonction utilitaire pour savoir si une tuile bloque le passage
int is_solid(int tileId) {
    // On considère que 6, 7, 8 (ton sol) et 28, 29, 30 (ta terre) sont solides
    return (tileId == 6 || tileId == 7 || tileId == 8 || tileId >= 28);
}

void HorizontaleCollision(Player* player, int* tileMap, int moveX) {
    player->rect.x += moveX;

    // Calcul des tuiles à vérifier (on transforme les pixels en index de grille)
    int tile_size = TILE_SIZE * MAP_SCALE;
    int leftTile = player->rect.x / tile_size;
    int rightTile = (player->rect.x + player->rect.w - 1) / tile_size;
    int topTile = player->rect.y / tile_size;
    int bottomTile = (player->rect.y + player->rect.h - 1) / tile_size;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                if (is_solid(tileMap[y * MAP_WIDTH + x])) {
                    // Si on allait à droite, on colle le joueur à gauche du mur
                    if (moveX > 0) player->rect.x = x * tile_size - player->rect.w;
                    // Si on allait à gauche, on colle le joueur à droite du mur
                    if (moveX < 0) player->rect.x = (x + 1) * tile_size;
                }
            }
        }
    }
}

void VerticaleCollision(Player* player, int* tileMap) {
    player->rect.y += (int)player->velocityY;

    int tile_size = TILE_SIZE * MAP_SCALE;
    int leftTile = player->rect.x / tile_size;
    int rightTile = (player->rect.x + player->rect.w - 1) / tile_size;
    int topTile = player->rect.y / tile_size;
    int bottomTile = (player->rect.y + player->rect.h - 1) / tile_size;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                if (is_solid(tileMap[y * MAP_WIDTH + x])) {
                    if (player->velocityY > 0) { // Chute
                        player->rect.y = y * tile_size - player->rect.h;
                        player->velocityY = 0;
                        player->isJumping = 0;
                    }
                    else if (player->velocityY < 0) { // Saut (cogne le plafond)
                        player->rect.y = (y + 1) * tile_size;
                        player->velocityY = 0;
                    }
                }
            }
        }
    }
}