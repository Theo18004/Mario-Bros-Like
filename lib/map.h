#include <SDL2/SDL.h>

// On définit la taille des blocs ici pour qu'elle soit accessible partout
#define TILE_SIZE 40

/**
 * Charge une carte à partir d'une image.
 * @param filename : chemin du fichier image (ex: "assets/map.png")
 * @param platforms : pointeur vers le tableau de rectangles qui sera alloué
 * @param count : pointeur pour stocker le nombre de plateformes créées
 * @param playerPos : pointeur pour mettre à jour la position de départ du joueur
 */
void loadMapFromImage(const char* filename, SDL_Rect** platforms, int* count, SDL_Rect* playerPos);

