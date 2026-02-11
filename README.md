# 🎮 MarioBrosLike


> **Projet étudiant de L2 Informatique** - Université du Mans.  
> *Début du développement : 13 janvier 2025*

Ce projet est un moteur de jeu de plateforme rétro performant écrit en C avec la bibliothèque SDL2. Il implémente les fondamentaux d'un jeu 2D : physique, collisions, tilemapping et entités.

---

## 👥 L'Équipe

| Membre | Formation | Établissement |
| :--- | :---: | :---: |
| **Angot Mathis** | L2 Informatique | Le Mans Université |
| **Anfray Alban** | L2 Informatique | Le Mans Université |
| **Caillon Elouan** | L2 Informatique | Le Mans Université |
| **Ernault Théo** | L2 Informatique | Le Mans Université |

---

##  Fonctionnalités

Le moteur intègre les mécaniques suivantes :

* ** Déplacements Fluides** : Gestion précise de la physique du joueur (course avec inertie) et sauts paraboliques avec une gravité réaliste.
* ** Système de Tilemap** : Chargement et rendu de niveaux basés sur des tuiles.
* ** Collisions Précises** : Détection et résolution des collisions avec le sol, les murs et les plateformes suspendues.
* ** Ennemis (IA Simple)** : Entités autonomes patrouillant avec détection automatique des bords de plateformes.
* ** Game Loop Optimisée** : Gestion du *Delta Time* pour assurer une vitesse de jeu constante, indépendamment du framerate de la machine.

---

##  Structure du Projet

```bash
.
├── assets/          # Sprites, tuiles et images (.png)
├── include/         # Fichiers d'en-tête (.h)
├── src/             # Code source (.c)
│   ├── main.c       # Point d'entrée et boucle de jeu principale
│   ├── player.c     # Logique et physique du joueur
│   ├── map.c        # Gestion du chargement et rendu de la Tilemap
│   └── physics.c    # Moteur de collisions (AABB)
├── Makefile         # Script d'automatisation de la compilation
└── README.md        # Documentation du projet
