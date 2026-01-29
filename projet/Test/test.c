#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    IMG_Init(imgFlags);

    SDL_Window* window = SDL_CreateWindow(
        "Platformer Mario-Style Camera",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "charlie.gif");

    // --- OBJETS DU JEU (Coordonnées réelles dans le monde) ---
    SDL_Rect player = {100, 400, 50, 50};
    SDL_Rect platform1 = {300, 350, 200, 20};
    SDL_Rect platform2 = {600, 250, 200, 20}; // Placée plus loin à droite
    SDL_Rect platform3 = {900, 350, 200, 20}; // En dehors de l'écran au départ
    
    // Le sol est très large (2000 pixels) pour pouvoir marcher longtemps
    SDL_Rect ground = {0, 500, 3000, 100};

    float velY = 0.0f;
    float gravity = 0.5f;
    int canJump = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE; 

    // VARIABLE CAMÉRA
    int scrollX = 0;

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        // Déplacements
        if (keys[SDL_SCANCODE_LEFT]) {
            player.x -= 5;
            flip = SDL_FLIP_HORIZONTAL;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            player.x += 5;
            flip = SDL_FLIP_NONE;
        }

        // Empêcher de sortir à gauche du monde
        if (player.x < 0) player.x = 0;

        // Saut
        if (keys[SDL_SCANCODE_UP] && canJump) {
            velY = -12.0f;
            canJump = 0;
        }

        // Physique
        velY += gravity;
        player.y += (int)velY;
        canJump = 0; 

        // --- GESTION DE LA CAMÉRA ---
        // La caméra commence à bouger quand le joueur dépasse le milieu de l'écran (400px)
        if (player.x > WINDOW_WIDTH / 2) {
            scrollX = player.x - WINDOW_WIDTH / 2;
        } else {
            scrollX = 0;
        }

        // --- COLLISIONS (Toujours avec les coordonnées réelles) ---
        if (player.y >= ground.y - player.h) {
            player.y = ground.y - player.h;
            velY = 0;
            canJump = 1;
        }

        // Test de collision simplifié pour les plateformes
        SDL_Rect plats[] = {platform1, platform2, platform3};
        for(int i = 0; i < 3; i++) {
            if (SDL_HasIntersection(&player, &plats[i])) {
                if (velY > 0) { 
                    player.y = plats[i].y - player.h;
                    velY = 0;
                    canJump = 1;
                }
            }
        }

        // --- RENDU (On utilise scrollX ici) ---
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); 
        SDL_RenderClear(renderer);

        // Calcul des positions d'affichage (Position Réelle - scrollX)
        SDL_Rect drawGround = { ground.x - scrollX, ground.y, ground.w, ground.h };
        SDL_Rect drawPlat1  = { platform1.x - scrollX, platform1.y, platform1.w, platform1.h };
        SDL_Rect drawPlat2  = { platform2.x - scrollX, platform2.y, platform2.w, platform2.h };
        SDL_Rect drawPlat3  = { platform3.x - scrollX, platform3.y, platform3.w, platform3.h };
        SDL_Rect drawPlayer = { player.x - scrollX, player.y, player.w, player.h };

        // Dessin du sol
        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
        SDL_RenderFillRect(renderer, &drawGround);

        // Dessin des plateformes
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
        SDL_RenderFillRect(renderer, &drawPlat1);
        SDL_RenderFillRect(renderer, &drawPlat2);
        SDL_RenderFillRect(renderer, &drawPlat3);

        // Dessin du joueur
        if (playerTexture) {
            SDL_RenderCopyEx(renderer, playerTexture, NULL, &drawPlayer, 0, NULL, flip);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
            SDL_RenderFillRect(renderer, &drawPlayer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    IMG_Quit(); 
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}