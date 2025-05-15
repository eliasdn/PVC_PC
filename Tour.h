#ifndef TOUR_H
#define TOUR_H

#include <vector>
#include <iostream>
#include <numeric> // Pour std::accumulate ou simplement une boucle

// Déclaration anticipée de la classe Graph pour éviter une dépendance circulaire
class Graph;

class Tour {
public:
    // Constructeur : prend la séquence de nœuds et une référence au graphe
    // pour calculer la distance totale
    Tour(const std::vector<int>& nodes, const Graph& graph);

    // Getters
    const std::vector<int>& getNodes() const { return nodes_; }
    int getTotalDistance() const { return totalDistance_; }

    // Affiche la séquence des nœuds de la tournée et sa distance totale
    void print() const;

private:
    std::vector<int> nodes_; // Séquence des indices des nœuds
    int totalDistance_;      // Distance totale de la tournée
};

#endif