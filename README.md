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

* **Déplacements Fluides** : Gestion précise de la physique du joueur (course avec inertie) et sauts paraboliques avec une gravité réaliste.
* **Système de Tilemap** : Chargement et rendu de niveaux basés sur des tuiles.
* **Collisions Précises** : Détection et résolution des collisions avec le sol et les plateformes.
* **Game Loop Optimisée** : Boucle de jeu fluide avec gestion du temps et synchronisation verticale (VSync).
* **Animations Dynamiques** : Machine à états gérant les sprites (Idle, Run, Jump) et la direction du regard.

---

## 📂 Structure du Projet

```bash
.
├── assets/          # Sprites, tuiles et images (.png)
├── include/         # Fichiers d'en-tête (.h)
├── src/             # Code source (.c)
│   ├── main.c       # Point d'entrée et boucle de jeu principale
│   ├── player.c     # Logique et physique du joueur
│   ├── map.c        # Gestion du chargement et rendu de la Tilemap
│   ├── caméra.c        # Gestion de la caméra
│   └── collision.c    # Moteur de collisions (AABB)
├── Makefile         # Script d'automatisation de la compilation
└── README.md        # Documentation du projet
```

## 🛠️ Prérequis

Pour compiler et exécuter ce projet, vous devez disposer des bibliothèques de développement SDL2 installées sur votre machine.
Linux (Debian/Ubuntu/Mint)

```bash
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev
```

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
| <kbd>←</kbd> <kbd>→</kbd> | **Mouvement** | Courir vers la gauche ou la droite |
| <kbd>↑</kbd> | **Saut** | Sauter |
| <kbd>↓</kbd> | **Fast Fall** | Accélérer la chute une fois en l'air |
| <kbd>Echap</kbd> | **Quitter** | Fermer instantanément le jeu et revenir au bureau


⚙️ Fonctionnement Technique
Chargement de Niveau par Image (temporaire)

Machine à États (Animation)

Le système d'animation gère les transitions entre les sprites selon l'état physique du joueur :

    IDLE : Le joueur est immobile au sol.

    RUN : Le joueur se déplace horizontalement.

    JUMP : Le joueur est en l'air (vélocité Y négative ou positive).

Chaque état possède sa propre vitesse d'animation et son nombre de frames, gérés par un timer interne (frameTimer).
