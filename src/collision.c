#include "collision.h"
#include "defs.h"

int is_full(int tile_id) {
    if ((tile_id >= 6 && tile_id <= 8)   || 
        (tile_id >= 28 && tile_id <= 30) || 
        (tile_id >= 50 && tile_id <= 52) || 
        tile_id == 34 || tile_id == 264 || 
        tile_id == 265 || tile_id == 286 || 
        tile_id == 308 || tile_id == 309) {
        return 1;
    }
    return 0;
}

int is_slope_up_right(int tile_id) {
    return (tile_id == 268); // Monte vers la droite 
}

int is_slope_up_left(int tile_id) {
    return (tile_id == 269); // Monte vers la gauche 
}

int get_slope(int worldX, int tileX, int tileY, int tile_id) {
    int scaled_tile = TILE_SIZE * MAP_SCALE;
    int relX = worldX - (tileX * scaled_tile);

    // Clamp relX pour éviter les débordements 
    if (relX < 0) relX = 0;
    if (relX >= scaled_tile) relX = scaled_tile - 1;

    // Calcul de la position Y à l'intérieur de la tuile en fonction de la pente
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
                
                // Collision bloc plein
                if (is_full(tile)) return 1;

                // Collision Pente
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