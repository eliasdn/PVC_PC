#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

class Graph {
public:
    // Constructeur prenant la dimension et la matrice de distances
    Graph(int dimension, const std::vector<std::vector<int>>& distanceMatrix);
    
    // Destructeur
    ~Graph();

    // Getters pour accéder aux données du graphe
    int getDimension() const { return dimension_; }

    // Obtient la distance entre deux nœuds
    int getDistance(int i, int j) const;

    // Affiche la matrice de distances
    void printDistanceMatrix() const;

private:
    int dimension_;
    std::vector<std::vector<int>> distanceMatrix_;

    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
};

#endif