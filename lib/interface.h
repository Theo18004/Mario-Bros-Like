#ifndef INTERFACE_H
#define INTERFACE_H

//Affiche le nombre de vies du joueur en haut à droite de l'écran
void render_lives(SDL_Renderer* renderer, SDL_Texture* texHeart, int lives);

//Affiche la barre de progression en haut au centre de l'écran
void render_progress_bar(SDL_Renderer* renderer, int playerX, int mapPixelWidth);

#endif