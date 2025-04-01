# Variables de configuration
CXX = g++                    # Compilateur
CXXFLAGS = -Wall -g -std=c++11  # Options de compilation
INCLUDES = -Iinclude         # Chemin pour les fichiers d'en-tête

# Répertoires
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = base_donnees

# Nom de l'exécutable
TARGET = $(BIN_DIR)/gestion_employe

# Liste des fichiers source et objets
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)  # Tous les fichiers .cpp dans src/
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))  # Fichiers objets correspondants

# Règle par défaut : compiler tout
all: dirs $(TARGET)

# Créer les répertoires nécessaires
dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR)

# Lier les fichiers objets pour créer l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compiler chaque fichier source en fichier objet
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Exécuter le programme
run: all
	./$(TARGET)

# Nettoyer les fichiers générés
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Nettoyer et supprimer aussi le répertoire de données (optionnel)
distclean: clean
	rm -rf $(DATA_DIR)

# Indiquer que ces cibles ne sont pas des fichiers
.PHONY: all dirs run clean distclean