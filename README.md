# 🎮 MarioBrosLike

> **Projet étudiant de L2 Informatique** - Université du Mans.  
> *Début du développement : 13 janvier 2026*

Ce projet est un moteur de jeu de plateforme rétro performant écrit en C avec la bibliothèque SDL2. Il implémente les fondamentaux d'un jeu 2D : physique, collisions, tilemapping et entités animées.

---

## 👥 L'Équipe

| Membre | Formation | Établissement |
| :--- | :---: | :---: |
| **Angot Mathis** | L2 Informatique | Le Mans Université |
| **Anfray Alban** | L2 Informatique | Le Mans Université |
| **Caillon Elouan** | L2 Informatique | Le Mans Université |
| **Ernault Théo** | L2 Informatique | Le Mans Université |

---

## ✨ Fonctionnalités

Le moteur intègre les mécaniques suivantes :

* **Déplacements Fluides & Biomes** : Gestion précise de la physique du joueur (inertie, sauts paraboliques, gravité). La physique s'adapte au terrain (ex: glissades sur la glace, apesanteur lunaire).
* **Système de Tilemap (CSV)** : Chargement et rendu de niveaux créés via l'éditeur *Tiled*.
* **Collisions Précises** : Détection et résolution des collisions avec le sol, les murs et les demi-plateformes.
* **Game Loop Optimisée** : Boucle de jeu fluide avec gestion du temps.
* **Animations Dynamiques** : Machine à états gérant les sprites (Idle, Run, Jump, Dead) et la direction du regard.
* **Système d'Ennemis Varié** : Gestion des mouvements de patrouille, machines à états complexes (Thwomp, Presse) et interactions avec le joueur.
* **Interface & Audio** : Menus interactifs (Accueil, Pause, Paramètres), HUD (score, vies, timer) et intégration de musiques/bruitages avec SDL_mixer.

---

## 📂 Structure du Projet

```bash
## 📂 Structure du Projet

```bash
.
├── assets/          # Sprites, tuiles, images, musiques, sons et polices (.ttf)
├── bin/             # Exécutable final du jeu généré après la compilation
├── debogage/        # Fichiers, logs ou outils utilisés pour le débogage
├── doc/             # Documentation du code (générée via Doxygen dans doc/html)
├── lib/             # Bibliothèques externes et dépendances (fichiers .dll, .a)
├── obj/             # Fichiers objets (.o) générés lors de la compilation
├── src/             # Fichiers de code source (.c)
│   ├── camera.c     # Gestion du déplacement et du focus de la caméra
│   ├── collision.c  # Moteur de collisions (sol, pentes, demi-plateformes, etc)
│   ├── ennemi.c     # Logique, comportements (Machine à états) et rendu des ennemis
│   ├── flag.c       # Gestion du drapeau de fin de niveau
│   ├── interface.c  # HUD en jeu, écrans de victoire et Game Over
│   ├── Items.c      # Gestion des objets interactifs (Pièces, Checkpoints)
│   ├── level.c      # Initialisation des niveaux et gestion globale des maps
│   ├── main.c       # Point d'entrée, initialisation globale et boucle de jeu
│   ├── map.c        # Lecture du CSV, rendu des tuiles (tileset) et effets de parallaxe
│   ├── menu.c       # Gestion de l'écran titre et du menu des paramètres
│   ├── mort.c       # Gestion du respawn, perte de vies et conditions de mort
│   └── player.c     # Logique, contrôles, animations et physique du joueur
├── test/            # Programmes et scripts de tests unitaires
├── .gitignore       # Règles d'exclusion pour le dépôt Git
├── config.txt       # Fichier de sauvegarde locale (paramètres, luminosité)
├── Diagramme de Gantt actuel.ods       # Suivi d'avancement réel du projet
├── Diagramme de Gantt prévisionel.ods  # Planning initial du projet
├── Doxygen          # Fichier de configuration pour la génération de documentation
├── Makefile         # Script d'automatisation de la compilation
├── README.md        # Fichier de présentation du projet
├── To Do List.md    # Liste des tâches et fonctionnalités à implémenter
└── Tout les Crédits Mario-Bros-Like.md # Sources et crédits des assets utilisés
```

## 🛠️ Prérequis

Pour compiler et exécuter ce projet, vous devez disposer des bibliothèques de développement SDL2 installées sur votre machine.
Linux (Debian/Ubuntu/Mint)

```bash
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```
📥 Téléchargement et Installation

Avant de compiler, récupérez les sources du projet en clonant le dépôt :
Bash

git clone https://github.com/Theo18004/Mario-Bros-Like.git
cd Mario-Bros-Like

🚀 Compilation & Lancement

Le projet utilise un Makefile pour automatiser la compilation et la gestion des ressources.

Compilation : Ouvrez un terminal à la racine du projet et lancez la commande :

```Bash
make
```
Cela générera l'exécutable dans le dossier bin/.

Lancement : Pour jouer, utilisez la commande suivante (elle assure que le jeu trouve bien le dossier assets/) :
```Bash
make run
```
Nettoyage : Pour supprimer les fichiers temporaires (.o) et l'exécutable :
```Bash
make clean
```
Note importante : Ne lancez pas l'exécutable en faisant cd bin puis ./MarioBrosLike. Le jeu a besoin d'être lancé depuis la racine pour charger les images situées dans assets/.



## 🕹️ Contrôles

| Touche | Action | Description |
| :---: | :--- | :--- |
| <kbd>Q</kbd> <kbd>D</kbd> ou <kbd>←</kbd> <kbd>→</kbd> | **Mouvement** | Courir vers la gauche ou la droite |
| <kbd>Z</kbd> <kbd>↑</kbd> | **Saut** | Sauter |
| <kbd>P</kbd> | **Pause** | Affiche le menu pause en jeu |
| <kbd>Echap</kbd> | **Quitter** | Fermer instantanément le jeu et revenir au bureau


⚙️ Fonctionnement Technique

Le moteur lit des fichiers CSV générés par le logiciel Tiled. Chaque ID dans le CSV correspond à une tuile spécifique de la spritesheet (Tileset), ce qui permet de générer des niveaux vastes et de calculer les collisions de manière optimisée via une grille.

Machine à États (Animation)

Le système d'animation gère les transitions entre les sprites selon l'état physique du joueur :

    IDLE : Le joueur est immobile au sol.

    RUN : Le joueur se déplace horizontalement.

    JUMP : Le joueur est en l'air (vélocité Y négative ou positive).

    DEAD : Le joueur tombe et se couche sur le sol. 

Chaque état possède sa propre vitesse d'animation et son nombre de frames, gérés par un timer interne (SDL_GetTicks()).
