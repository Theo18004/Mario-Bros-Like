/**
 * @file collision.c
 * @brief Moteur de détection de collisions (AABB et pentes).
 */
#include "collision.h"
#include "defs.h"

/**
 * @brief Identifie si un ID de tuile correspond à un mur plein.
 * @param tile_id ID de la tuile à tester.
 * @return 1 si solide, 0 sinon.
 */
int is_full(int tile_id) {
    if ((tile_id >= 6 && tile_id <= 8)|| 
        (tile_id >= 28 && tile_id <= 30)|| 
        (tile_id >= 50 && tile_id <= 52)|| 
        tile_id == 34 || tile_id == 264 || 
        tile_id == 265 || tile_id == 286 || 
        tile_id == 308 || tile_id == 309 ||
        tile_id == 12 || tile_id == 14){
        return 1;
    }
    return 0;
}

int is_slope_up_right(int tile_id) { return (tile_id == 268); }
int is_slope_up_left(int tile_id) { return (tile_id == 269); }

/**
 * @brief Calcule la hauteur Y exacte sur une tuile de pente.
 * @param worldX Position X absolue du joueur.
 */
int get_slope(int worldX, int tileX, int tileY, int tile_id) {
    int scaled_tile = TILE_SIZE * MAP_SCALE;
    int relX = worldX - (tileX * scaled_tile);

    if (relX < 0) relX = 0;
    if (relX >= scaled_tile) relX = scaled_tile - 1;

    int y_inside;
    if (is_slope_up_right(tile_id)) {
        y_inside = scaled_tile - relX; 
    } else if (is_slope_up_left(tile_id)) {
        y_inside = relX;               
    } else {
        y_inside = scaled_tile;
    }
    return (tileY * scaled_tile) + y_inside;
}

int check_collision(SDL_Rect rect, int* map) {
    int scaled_tile = TILE_SIZE * MAP_SCALE;
    int left_tile   = rect.x / scaled_tile;
    int right_tile  = (rect.x + rect.w - 1) / scaled_tile;
    int top_tile    = rect.y / scaled_tile;
    int bottom_tile = (rect.y + rect.h - 1) / scaled_tile;

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                int tile = map[y * MAP_WIDTH + x];
                if (is_full(tile)) return 1;

                if (is_slope_up_right(tile) || is_slope_up_left(tile)) {
                    int playerCenterX = rect.x + rect.w / 2;
                    int tileStartX = x * scaled_tile;
                    if (playerCenterX >= tileStartX && playerCenterX < tileStartX + scaled_tile) {
                        int floorY = get_slope(playerCenterX, x, y, tile);
                        if (rect.y + rect.h > floorY) return 1;
                    }
                }
            }
        }
    }
    return 0; 
}