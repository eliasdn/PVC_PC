#include "Graph.h"
#include <iomanip> // Pour std::setw

// Constructeur
Graph::Graph(int dimension, const std::vector<std::vector<int>>& distanceMatrix)
    : dimension_(dimension), distanceMatrix_(distanceMatrix) {
    // Les données sont copiées ici par le constructeur de copie de std::vector
    // lors de l'initialisation de distanceMatrix_.
}

// Destructeur
Graph::~Graph() {
}

// Obtient la distance entre deux nœuds
int Graph::getDistance(int i, int j) const {
    // Simple vérification des valeurs
    if (i >= 0 && i < dimension_ && j >= 0 && j < dimension_) {
        return distanceMatrix_[i][j];
    } else {
        std::cerr << "Erreur: Accès hors limites à la matrice de distances (" << i << ", " << j << ")" << std::endl;
        return -1;
    }
}

// Affiche la matrice de distances
void Graph::printDistanceMatrix() const {
    std::cout << "Matrice de distances (" << dimension_ << "x" << dimension_ << "):" << std::endl;
    for (int i = 0; i < dimension_; ++i) {
        for (int j = 0; j < dimension_; ++j) {
            std::cout << std::setw(5) << distanceMatrix_[i][j] << " "; // std::setw pour aligner
        }
        std::cout << std::endl;
    }
}