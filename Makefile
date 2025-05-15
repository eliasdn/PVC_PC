# Définition du compilateur C++
CXX = g++

# Définition des options de compilation
# -Wextra : Activer des avertissements supplémentaires
# -g : Inclure les informations de débogage
CXXFLAGS = -std=c++14 -Wall -Wextra -g

# Liste des fichiers sources (.cpp) | idée : *.cpp
SRCS = main.cpp TsplibParser.cpp Graph.cpp TspSolver.cpp Tour.cpp

# Liste des fichiers objets (.o) correspondants
# La fonction patsubst remplace l'extension .cpp par .o dans la liste SRCS
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

# Nom de l'exécutable final
TARGET = tsp_solver

# Règle par défaut (construire l'exécutable)
all: $(TARGET)

# Règle pour construire l'exécutable à partir des fichiers objets
# $(TARGET) dépend de $(OBJS)
# La commande utilise le compilateur pour lier les fichiers objets
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(CXXFLAGS)

# Règle générique pour compiler un fichier .cpp en .o
# %.o : indique que cette règle s'applique à tous les fichiers .o
# %.cpp : indique que le fichier source correspondant a l'extension .cpp
# $< : Variable automatique qui représente le premier prérequis (le fichier .cpp)
# $@ : Variable automatique qui représente la cible (le fichier .o)
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Règle pour nettoyer le projet
# 'clean' est un nom de cible conventionnel pour supprimer les fichiers générés
clean:
	rm -f $(OBJS) *.tour
# Supprime les fichiers objets, [l'exécutable {$(TARGET)}] et les fichiers .tour

# Déclarer les cibles qui ne correspondent pas à des noms de fichiers réels
.PHONY: all clean