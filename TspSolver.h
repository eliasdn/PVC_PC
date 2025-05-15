#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "Graph.h"
#include "Tour.h"

#include <vector>
#include <limits> // Pour std::numeric_limits
#include <iostream>

class TspSolver {
public:
    // Constructeur : prend une référence constante au graphe à résoudre
    TspSolver(const Graph& graph);

    // Méthode principale pour lancer la résolution du TSP
    // Retourne un objet Tour représentant la solution trouvée
    Tour solve() const;

private:
    const Graph& graph_; // Référence constante au graphe

    // Implémentation de l'algorithme du plus proche voisin
    Tour nearestNeighborSolve() const;

    // Empêcher la copie et l'assignation (le solver est lié à un graphe spécifique) pour le moment
    TspSolver(const TspSolver&) = delete;
    TspSolver& operator=(const TspSolver&) = delete;
};

#endif