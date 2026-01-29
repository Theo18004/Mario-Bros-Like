# Marios Bros Like/ Platformer 2D

Fonctionnalités

Ce projet implémente les mécaniques de base d'un platformer rétro :

· Déplacements Fluides :** Gestion de la course avec inertie et sauts paraboliques (gravité réaliste).
· Système de Tilemap :** Chargement et rendu de niveaux basés sur des tuiles (fichiers `.txt` ou tableaux).
· Collisions Précises :** Détection et résolution des collisions (AABB) avec le sol, les murs et les plateformes suspendues.
· Ennemis (IA Simple) :** Entités patrouillant automatiquement avec détection des bords.
· Game Loop Optimisée : ** Gestion du *delta time* pour assurer une vitesse de jeu constante peu importe le framerate.





Structure du Projet


├── assets/          # Sprites, tuiles et images
├── src/             # Code source (.c)
│   ├── main.c       # Point d'entrée et Game Loop
│   ├── player.c     # Logique du joueur
│   ├── map.c        # Gestion de la Tilemap
│   └── physics.c    # Moteur de collisions
├── include/         # En-têtes (.h)
├── Makefile         # Script de compilation
└── README.md        # Documentation


Installation et Lancement

1. Prérequis

Vous devez avoir installé **GCC** et la bibliothèque **SDL2** (ainsi que `SDL2_image` si tu l'utilises pour les sprites).

· Linux (Debian/Ubuntu) :
    ```bash
    sudo apt-get update
    sudo apt-get install build-essential libsdl2-dev
    ```

· macOS (via Homebrew) :
    ```bash
    brew install sdl2
    ```

· Windows :
    Utilisez MSYS2 ou configurez les liens SDL2 dans votre IDE (VS Code / Visual Studio).

2. Compilation

Clonez le dépôt et compilez le projet via le Makefile :

```bash
git clone [https://github.com/ton-pseudo/nom-du-repo.git](https://github.com/ton-pseudo/nom-du-repo.git)
cd nom-du-repo
make
