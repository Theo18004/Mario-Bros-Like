# Nom de l'exécutable
TARGET = bin/MarioBrosLike

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Ilib -g $(shell pkg-config --cflags sdl2 SDL2_image)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image) -lm

# Dossiers
SRC_DIR = src
OBJ_DIR = bin

# Sources et Objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

# Édition de liens (Juste la compilation, PAS de copie d'assets)
$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation des .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# --- COMMANDE POUR LANCER LE JEU ---
# Tapez "make run" pour jouer. 
# Cela lance l'exécutable depuis la racine, donc il verra le dossier "assets"
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean run