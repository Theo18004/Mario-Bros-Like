/**
 * @file map.h
 * @brief Gestion du chargement de la carte et des niveaux.
 * * Ce module permet de transformer une image bitmap en une liste de 
 * rectangles SDL exploitables par le moteur de collisions.
 */

#include <SDL2/SDL.h>

// On définit la taille des blocs ici pour qu'elle soit accessible partout
#define TILE_SIZE 40

/**
 * @brief Charge une carte à partir d'une image bitmap.
 * * Cette fonction analyse chaque pixel d'une image :
 * - Un pixel **noir** (RGB 0,0,0) génère une plateforme SDL_Rect.
 * - Un pixel **bleu** (RGB 0,0,255) définit la position de départ du joueur.
 * * @param filename Chemin du fichier image (ex: "assets/map.png").
 * @param platforms Pointeur vers le tableau de rectangles qui sera alloué dynamiquement par la fonction.
 * @param count Pointeur vers un entier pour stocker le nombre de plateformes créées.
 * @param playerPos Pointeur vers un SDL_Rect pour mettre à jour la position initiale du joueur.
 * * @note Il incombe à l'appelant de libérer la mémoire allouée pour @p platforms avec free().
 * @warning Si l'image n'est pas trouvée ou si l'allocation échoue, un message d'erreur est affiché sur stdout.
 */
void loadMapFromImage(const char* filename, SDL_Rect** platforms, int* count, SDL_Rect* playerPos);

