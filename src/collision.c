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
int is_full(int tile_id, int levelID) {
    // --- MAP 1 ---
    if (levelID == 1) {
        if ((tile_id >= 6 && tile_id <= 10)|| 
            (tile_id >= 28 && tile_id <= 32)|| 
            (tile_id >= 50 && tile_id <= 52)|| 
            tile_id == 34 || tile_id == 286 || 
            tile_id == 279 || tile_id == 301 ||
            tile_id == 286 || tile_id == 288 ||
            (tile_id >= 264 && tile_id <= 266) ||
            (tile_id >= 308 && tile_id <= 310) ||
            (tile_id >= 352 && tile_id <= 354) ||
            (tile_id >= 396 && tile_id <= 398) ||
            (tile_id >= 278 && tile_id <= 280) ||
            (tile_id >= 282 && tile_id <= 284) ||
            (tile_id >= 300 && tile_id <= 302) ||
            (tile_id >= 322 && tile_id <= 324) ||
            tile_id == 12 || tile_id == 14){
            return 1;
        }
    }
    // --- MAP 2 ---
    else if (levelID == 2) {
        if((tile_id >= 0 && tile_id <= 11)||
            (tile_id >= 22 && tile_id <= 33)||
            (tile_id >= 44 && tile_id <= 55)||
            (tile_id >= 66 && tile_id <= 77)||
            (tile_id >= 88 && tile_id <= 99)||
            (tile_id >= 110 && tile_id <= 121)||
            (tile_id >= 132 && tile_id <= 143)||
            (tile_id >= 154 && tile_id <= 165)||
            (tile_id >= 180 && tile_id <= 189)||
            (tile_id >= 374 && tile_id <= 376)||
            (tile_id >= 378 && tile_id <= 380)||
            (tile_id >= 396 && tile_id <= 398)||
            (tile_id >= 418 && tile_id <= 420)||
            (tile_id >= 506 && tile_id <= 510)||
            (tile_id >= 528 && tile_id <= 532)||
            (tile_id >= 550 && tile_id <= 554)||
            (tile_id >= 572 && tile_id <= 576)||
            tile_id == 402 ){
            return 1;
            }
    }
    return 0;
}

int is_slope_up_right(int tile_id, int levelID) { 
    if ( levelID == 1) {
        return (tile_id == 268 || tile_id == 304); 
    }else if (levelID == 2) {
        return (tile_id == 177 || tile_id == 198); 
    } 
    return 0;
}
int is_slope_up_left(int tile_id, int levelID) {
    if ( levelID == 1) {
        return (tile_id == 269 || tile_id == 303); 
    }else if (levelID == 2) {
        return (tile_id == 178 || tile_id == 201); 
    }
    return 0;
}

/**
 * @brief Calcule la hauteur Y exacte sur une tuile de pente.
 * @param worldX Position X absolue du joueur.
 */
int get_slope(int worldX, int tileX, int tileY, int tile_id, int levelID) {
    int scaled_tile = TILE_SIZE * MAP_SCALE;
    int relX = worldX - (tileX * scaled_tile);

    if (relX < 0) relX = 0;
    if (relX >= scaled_tile) relX = scaled_tile - 1;

    int y_inside;
    if (is_slope_up_right(tile_id, levelID)) {
        y_inside = scaled_tile - relX; 
    } else if (is_slope_up_left(tile_id, levelID)) {
        y_inside = relX;               
    } else {
        y_inside = scaled_tile;
    }
    return (tileY * scaled_tile) + y_inside;
}


int demi_plate_sol(int tile_id, int levelID) { 
    if ( levelID == 1){
        return ((tile_id >= 462 && tile_id <= 464) || (tile_id >= 440 && tile_id <= 442));
        printf('demi plate'); 
    }
    else if (levelID == 2) {
        return 0; 
        printf('pas de demi plate'); 
    }
    return 0;
}

int demi_plate(SDL_Rect rect, int* map, int levelID) {
    int scaled_tile = TILE_SIZE * MAP_SCALE;
    int left_tile   = rect.x / scaled_tile;
    int right_tile  = (rect.x + rect.w - 1) / scaled_tile;
    int top_tile    = rect.y / scaled_tile;
    int bottom_tile = (rect.y + rect.h - 1) / scaled_tile;

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                int tile = map[y * MAP_WIDTH + x];

                if (demi_plate_sol(tile, levelID)) {
                    int tile_top = y * scaled_tile;
                    int tile_limit = tile_top + (int)(scaled_tile );
                    if (rect.y < tile_limit && (rect.y + rect.h) > tile_top) {
                        return 1; 
                    }
                }
            }
        }
    }
    return 0; 
}

int check_collision(SDL_Rect rect, int* map, int check_demi, int levelID) {
    int scaled_tile = TILE_SIZE * MAP_SCALE;
    int left_tile   = rect.x / scaled_tile;
    int right_tile  = (rect.x + rect.w - 1) / scaled_tile;
    int top_tile    = rect.y / scaled_tile;
    int bottom_tile = (rect.y + rect.h - 1) / scaled_tile;
    int on_slope = 0; 

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                int tile = map[y * MAP_WIDTH + x];
                
                if (is_slope_up_right(tile, levelID) || is_slope_up_left(tile, levelID)) {
                    int playerCenterX = rect.x + rect.w / 2;
                    int tileStartX = x * scaled_tile;
                    
                    if (playerCenterX >= tileStartX && playerCenterX < tileStartX + scaled_tile) {
                        int floorY = get_slope(playerCenterX, x, y, tile, levelID);
                        if (rect.y + rect.h > floorY) {
                            return 1; 
                        }
                        on_slope = 1; 
                    }
                }
            }
        }
    }

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                int tile = map[y * MAP_WIDTH + x];
                
                if (is_full(tile, levelID)) {
                    if (on_slope) {
                        continue; 
                    }
                    return 1; 
                }
                if (demi_plate_sol(tile, levelID)) {
                    if (check_demi == 1) {
                        int tile_top = y * scaled_tile;
                        if (rect.y + rect.h > tile_top && rect.y + rect.h <= tile_top + 25) {
                            return 1; 
                        }
                    }
                }
            }
        }
    }

    return 0; 
}

