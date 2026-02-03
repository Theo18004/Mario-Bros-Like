# Nom de l'exécutable
TARGET = bin/MarioBrosLike

# Compilateur et options
CC = gcc
# On ajoute pkg-config pour récupérer automatiquement les bons chemins SDL
CFLAGS = -Wall -Wextra -Ilib -g $(shell pkg-config --cflags sdl2 SDL2_image)
# On ajoute les liens vers les bibliothèques SDL2
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image) -lm

# Dossiers
SRC_DIR = src
OBJ_DIR = bin

# Sources et Objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean