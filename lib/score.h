#ifndef SCORE_H
#define SCORE_H

#include <SDL2/SDL.h>

typedef struct {
    int value;
    int max_x;
    SDL_Rect rect; 
} Score;

void init_score(Score* s, int x, int y);
void update_score(Score* s, int player_x);

void render_score(SDL_Renderer* renderer, Score* s);

#endif