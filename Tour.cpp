#include "Tour.h"
#include "Graph.h" // Inclusion complète du Graph dans le fichier .cpp
#include <algorithm> // Pour std::swap

// Constructeur
Tour::Tour(const std::vector<int>& nodes, const Graph& graph)
    : nodes_(nodes), totalDistance_(0) { // Initialise nodes_ et totalDistance_
    
    // Une tournée n'a de sens (et de distance > 0) qu'avec au moins 2 nœuds pour un aller-retour complet
    if (nodes_.size() < 2) {
        totalDistance_ = 0;
        return; // Pas de calcul nécessaire pour 0 ou 1 nœud
    }

    // Calculer la distance totale de la tournée
    // Parcourir la séquence des nœuds
    for (size_t i = 0; i < nodes_.size() - 1; ++i) {
        int current_node = nodes_[i];
        int next_node = nodes_[i+1];
        // Ajouter la distance entre le nœud actuel et le suivant
        totalDistance_ += graph.getDistance(current_node, next_node);
    }

    // Ajouter la distance de retour du dernier nœud au premier (fermeture de la tournée)
    int last_node = nodes_.back(); // Le dernier élément du vecteur
    int first_node = nodes_.front(); // Le premier élément du vecteur
    totalDistance_ += graph.getDistance(last_node, first_node);
}

// Affiche la séquence des nœuds et la distance totale
void Tour::print() const {
    std::cout << "Tour (" << nodes_.size() << " nœuds) : ";
    for (size_t i = 0; i < nodes_.size(); ++i) {
        // Afficher les IDs des nœuds en 1-basé pour une meilleure lisibilité
        std::cout << nodes_[i] + 1 << (i == nodes_.size() - 1 ? "" : " -> ");
    }
    std::cout << std::endl;
    std::cout << "Distance totale : " << totalDistance_ << std::endl;
}

// Inverse une sous-séquence de la tournée
void Tour::reverseSubsequence(int start, int end) {
    if (start >= end) {
        return; // Rien à inverser
    }
    while (start < end) {
        std::swap(nodes_[start], nodes_[end]);
        start++;
        end--;
    }
}

// Recalcul la distance de l'objet
void Tour::recalculateDistance(const Graph& graph) {
    totalDistance_ = 0;
    if (nodes_.size() < 2) {
        return;
    }
    for (size_t i = 0; i < nodes_.size() - 1; ++i) {
        totalDistance_ += graph.getDistance(nodes_[i], nodes_[i + 1]);
    }
    totalDistance_ += graph.getDistance(nodes_.back(), nodes_.front());
}