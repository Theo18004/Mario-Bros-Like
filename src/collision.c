#include "collision.h"
#include "defs.h"

int is_solid(int tile_id) {
    // Liste ici tous tes ID de blocs solides (herbe, terre, metal, pierre...)
    if (tile_id == 6 || tile_id == 7 || tile_id == 8 || tile_id == 16 || tile_id == 17 || tile_id == 18 || tile_id == 28 || tile_id == 29 || tile_id == 30 || tile_id == 32 || tile_id == 33 || tile_id == 34 || tile_id == 46 || tile_id == 47 || tile_id == 48 || tile_id == 50 || tile_id == 51 || tile_id == 52) {
        return 1; 
    }
    return 0; 
}

int check_collision(SDL_Rect rect, int* map) {
    int left_tile   = rect.x / (TILE_SIZE * MAP_SCALE);
    int right_tile  = (rect.x + rect.w - 1) / (TILE_SIZE * MAP_SCALE);
    int top_tile    = rect.y / (TILE_SIZE * MAP_SCALE);
    int bottom_tile = (rect.y + rect.h - 1) / (TILE_SIZE * MAP_SCALE);

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                int tile = map[y * MAP_WIDTH + x];
                if (is_solid(tile)) return 1; 
            }
        }
    }
    return 0; 
}