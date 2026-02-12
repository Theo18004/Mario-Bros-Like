#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TILE_SIZE 16      
#define MAP_SCALE 2       
#define MAP_WIDTH 192   // Largeur de votre map v1.csv
#define MAP_HEIGHT 32   // Hauteur de votre map v1.csv

typedef struct {
    SDL_Texture* texture;
    int columns;          
    int tileWidth;
    int tileHeight;
} Tileset;

// --- CHARGEMENT DU CSV ---
// Remplit le tableau avec les IDs du fichier map_v1.csv 
int load_map_from_csv(const char* filename, int* map_array) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;

    char line[4096];
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < MAP_HEIGHT) {
        char* token = strtok(line, ",");
        int col = 0;
        while (token && col < MAP_WIDTH) {
            map_array[row * MAP_WIDTH + col] = atoi(token);
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    fclose(file);
    return 1;
}

// --- AFFICHAGE DES TUILES ---
// Gère le dessin et le décalage de la caméra (scrollX/scrollY) 
void draw_tile(SDL_Renderer* renderer, Tileset* ts, int tileId, int x, int y, int scrollX, int scrollY) {
    if (tileId < 0) return; // -1 = vide dans votre CSV 

    SDL_Rect src = { (tileId % ts->columns) * ts->tileWidth, (tileId / ts->columns) * ts->tileHeight, ts->tileWidth, ts->tileHeight };
    SDL_Rect dest = { x - scrollX, y - scrollY, ts->tileWidth * MAP_SCALE, ts->tileHeight * MAP_SCALE };

    SDL_RenderCopy(renderer, ts->texture, &src, &dest);
}

int main(int argc, char* argv[]) {
    int* tileMap = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
    if (!load_map_from_csv("map_v1.csv", tileMap)) return 1;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Rendu Map CSV", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int screenW, screenH;
    SDL_GetRendererOutputSize(renderer, &screenW, &screenH);

    SDL_Texture* terrainTex = IMG_LoadTexture(renderer, "assets/Terrain/Terrain (16x16).png");
    Tileset ts = { terrainTex, 22, 16, 16 };

    // --- CALCULS DE CAMÉRA ET COLLISIONS ---
    int mapTotalHeight = MAP_HEIGHT * TILE_SIZE * MAP_SCALE;
    int scrollY = mapTotalHeight - screenH; // Aligne le bas de la map à l'écran 
    int groundLevelY = 16 * TILE_SIZE * MAP_SCALE; // Le sol est à la ligne 16 

    SDL_Rect player = {100, groundLevelY - 48, 48, 48}; 
    float velY = 0.0f;
    int scrollX = 0, running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) running = 0;
        }

        // Physique simple et mouvement
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT]) player.x -= 5;
        if (keys[SDL_SCANCODE_RIGHT]) player.x += 5;
        if (keys[SDL_SCANCODE_SPACE] && player.y >= groundLevelY - player.h) velY = -12.0f;

        velY += 0.6f;
        player.y += (int)velY;

        // --- COLLISION SOL ---
        if (player.y >= groundLevelY - player.h) { 
            player.y = groundLevelY - player.h; 
            velY = 0; 
        }

        // Caméra suit le joueur
        if (player.x > screenW / 2) scrollX = player.x - screenW / 2;

        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
        SDL_RenderClear(renderer);

        // --- DESSIN DE LA MAP ---
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tileId = tileMap[y * MAP_WIDTH + x];
                draw_tile(renderer, &ts, tileId, x * TILE_SIZE * MAP_SCALE, y * TILE_SIZE * MAP_SCALE, scrollX, scrollY);
            }
        }

        // Rendu joueur (carré temporaire pour test collision)
        SDL_Rect playerDest = { player.x - scrollX, player.y - scrollY, player.w, player.h };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &playerDest);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    free(tileMap);
    SDL_DestroyTexture(terrainTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}