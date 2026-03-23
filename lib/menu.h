#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h> // Nécessaire pour Mix_Chunk

extern int volume;      
extern int luminosite;  

typedef enum {
    MENU_QUIT = 0,
    MENU_PLAY = 1,
    MENU_OPTIONS = 2 
} MenuResult;

void charger_parametres();
void sauvegarder_parametres();
MenuResult afficher_menu(SDL_Renderer* renderer, int width, int height);
void afficher_parametres(SDL_Renderer* renderer, int width, int height, TTF_Font* font, SDL_Texture* texBG);
MenuResult afficher_pause(SDL_Renderer* renderer, int width, int height, Mix_Chunk* sfx);

#endif