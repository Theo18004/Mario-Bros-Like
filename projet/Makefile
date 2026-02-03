# Nom de l'exécutable final
TARGET = bin/MarioBrosLike

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Ilib -g
LDFLAGS = -lm

# Dossiers
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = bin

# Recherche automatique des fichiers sources (.c) dans src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Génération de la liste des fichiers objets (.o) dans bin/
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Règle par défaut
all: $(TARGET)

# Liaison de l'exécutable
$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilation terminée : $(TARGET)"

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers temporaires
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
	@echo "Nettoyage effectué."

# Pour éviter les conflits avec des fichiers du même nom
.PHONY: all clean