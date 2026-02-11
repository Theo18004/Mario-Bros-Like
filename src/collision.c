#include <stdio.h>
#include <SDL/SDL.h>
#include <collision.h>
#include <player.h>
#include <stdlib.h>


/* 
 *  fonction de collision AABB
 */
int collision( SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

// ==============================
// COLLISION HORIZONTAL
// ==============================
void HorizontaleCollision(Player* player, SDL_Rect* platforms, int platformCount, int moveX) {
    player->rect.x += moveX;

    for (int i = 0; i < platformCount; i++)
    {
        if (collision(player->rect, platforms[i]))
        {
            if (moveX > 0) // droite
                player->rect.x = platforms[i].x - player->rect.w;

            else if (moveX < 0) // gauche
                player->rect.x = platforms[i].x + platforms[i].w;
        }
    }
}

// ==============================
// COLLISION VERTICAL
// ==============================
void VerticaleCollision(Player* player, SDL_Rect* platforms, int platformCount) {
    player->rect.y += (int)player->velocityY;

    for (int i = 0; i < platformCount; i++)
    {
        if (collision(player->rect, platforms[i]))
        {
            if (player->velocityY > 0) { // bas
                player->rect.y = platforms[i].y - player->rect.h;
                player->velocityY = 0;
                player->isJumping = 0;
            }
            else if (player->velocityY < 0) {   // haut
                player->rect.y = platforms[i].y + platforms[i].h;
                player->velocityY = 0;
            }
        }
    }
}
