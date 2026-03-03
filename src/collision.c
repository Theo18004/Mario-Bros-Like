#include "collision.h"
#include "defs.h"

int check_collision(SDL_Rect rect, int* map) {
    int left_tile   = rect.x / (TILE_SIZE * MAP_SCALE);
    int right_tile  = (rect.x + rect.w - 1) / (TILE_SIZE * MAP_SCALE);
    int top_tile    = rect.y / (TILE_SIZE * MAP_SCALE);
    int bottom_tile = (rect.y + rect.h - 1) / (TILE_SIZE * MAP_SCALE);

    for (int y = top_tile; y <= bottom_tile; y++) {
        for (int x = left_tile; x <= right_tile; x++) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                int tile = map[y * MAP_WIDTH + x];
                if (tile == TILE_SOLID) return 1;
            }
        }
    }
    return 0; 
}