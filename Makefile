# Nom de l'exécutable
TARGET = bin/MarioBrosLike

# Compilateur
CC = gcc

# --- CONFIGURATION DES FLAGS ---
# CFLAGS : Ajout de SDL2_ttf pour la compilation (trouve SDL_ttf.h)
CFLAGS = -Wall -Wextra -Ilib -g $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)

# LDFLAGS : Ajout de SDL2_ttf pour l'édition de liens (trouve la lib)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf) -lm

# Dossiers
SRC_DIR = src
OBJ_DIR = bin

# Sources et Objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

# Édition de liens
$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation des .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# --- COMMANDE POUR LANCER LE JEU ---
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean run