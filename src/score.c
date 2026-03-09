#include "score.h"
#include <SDL2/SDL.h>

// --- CONFIGURATION ---
#define PIXEL_SCALE 4   // Taille d'un "gros pixel" (plus c'est grand, plus le texte est gros)
#define SPACE_BETWEEN 4 // Espace entre deux chiffres

// Initialisation
void init_score(Score* s, int x, int y) {
    s->value = 0;
    s->max_x = 0;
    s->rect.x = x;
    s->rect.y = y;
    // La taille logique d'un chiffre (3 pixels de large * SCALE)
    s->rect.w = 3 * PIXEL_SCALE; 
    s->rect.h = 5 * PIXEL_SCALE; 
}


void update_score(Score* s, int player_x) {
    if (player_x > s->max_x) {
        s->max_x = player_x;
        s->value = s->max_x / 20; 
    }
}

// Fonction qui dessine un chiffre en Pixel Art
// On utilise une grille de 3 colonnes x 5 lignes
static void draw_pixel_digit(SDL_Renderer* renderer, int x, int y, int digit) {
    
    // Définition des motifs (1 = pixel allumé, 0 = pixel éteint)
    // 10 chiffres, 5 lignes, 3 colonnes
    const int font[10][5][3] = {
        { // 0
            {1,1,1},
            {1,0,1},
            {1,0,1},
            {1,0,1},
            {1,1,1}
        },
        { // 1
            {0,1,0},
            {1,1,0},
            {0,1,0},
            {0,1,0},
            {1,1,1}
        },
        { // 2
            {1,1,1},
            {0,0,1},
            {1,1,1},
            {1,0,0},
            {1,1,1}
        },
        { // 3
            {1,1,1},
            {0,0,1},
            {0,1,1},
            {0,0,1},
            {1,1,1}
        },
        { // 4
            {1,0,1},
            {1,0,1},
            {1,1,1},
            {0,0,1},
            {0,0,1}
        },
        { // 5
            {1,1,1},
            {1,0,0},
            {1,1,1},
            {0,0,1},
            {1,1,1}
        },
        { // 6
            {1,1,1},
            {1,0,0},
            {1,1,1},
            {1,0,1},
            {1,1,1}
        },
        { // 7
            {1,1,1},
            {0,0,1},
            {0,1,0},
            {0,1,0},
            {0,1,0}
        },
        { // 8
            {1,1,1},
            {1,0,1},
            {1,1,1},
            {1,0,1},
            {1,1,1}
        },
        { // 9
            {1,1,1},
            {1,0,1},
            {1,1,1},
            {0,0,1},
            {1,1,1}
        }
    };

    // Couleur du texte (BLANC ici)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Boucle pour dessiner les pixels
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 3; col++) {
            if (font[digit][row][col] == 1) {
                SDL_Rect pixelRect = {
                    x + (col * PIXEL_SCALE), // Position X
                    y + (row * PIXEL_SCALE), // Position Y
                    PIXEL_SCALE,             // Largeur
                    PIXEL_SCALE              // Hauteur
                };
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }
}

void render_score(SDL_Renderer* renderer, Score* s) {
    int tempScore = s->value;
    int numDigits = 0;
    
    // Calcul du nombre de chiffres
    if (tempScore == 0) numDigits = 1;
    else {
        int c = tempScore;
        while (c > 0) { c /= 10; numDigits++; }
    }

    // Calcul de la largeur totale du chiffre (3 colonnes * scale)
    int digitWidth = 3 * PIXEL_SCALE;

    // Position de départ (droite vers gauche)
    // On décale chaque chiffre de (largeur + espace)
    int currentX = s->rect.x + (numDigits - 1) * (digitWidth + SPACE_BETWEEN);

    if (s->value == 0) {
        draw_pixel_digit(renderer, currentX, s->rect.y, 0);
    } else {
        while (tempScore > 0) {
            int digit = tempScore % 10;
            draw_pixel_digit(renderer, currentX, s->rect.y, digit);
            
            // On décale vers la gauche pour le prochain chiffre
            currentX -= (digitWidth + SPACE_BETWEEN);
            tempScore /= 10;
        }
    }
}

void reset_score(Score* s) {
    if (s != NULL) {
        s->value = 0; // Ou le champ que tu utilises pour stocker le score
        // Réinitialise aussi la position si nécessaire
    }
}