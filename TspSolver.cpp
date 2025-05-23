#include "TspSolver.h"
#include <limits> // Nécessaire pour std::numeric_limits
#include <algorithm> // Potentiellement pour std::min, mais une simple comparaison suffit

// Constructeur
TspSolver::TspSolver(const Graph& graph) : graph_(graph) {
    // Le constructeur stocke simplement la référence au graphe.
}

// Méthode principale pour lancer la résolution
Tour TspSolver::solve() const {
    // Pour l'instant, solve() appelle simplement l'algorithme du plus proche voisin
    // Plus tard, on pourrait ajouter d'autres algorithmes ici et choisir lequel exécuter
    Tour result_tour = nearestNeighborSolve(0);
    for(int i = 1; i<graph_.getDimension(); ++i) {
        Tour tour_i = nearestNeighborSolve(i);
        if (tour_i.getTotalDistance() < result_tour.getTotalDistance())
            result_tour = tour_i;
    }

    // Amélioration du tour par une optimisation
    result_tour = OptimizationSwapEdges(result_tour);

    return result_tour;
}

// Implémentation de l'algorithme du plus proche voisin
Tour TspSolver::nearestNeighborSolve(int start_node) const {
    int dimension = graph_.getDimension();

    // Vérification de base pour un graphe valide
    if (dimension <= 1) {
        // Pour 0 ou 1 nœud, la tournée est triviale
        std::vector<int> trivial_tour;
        if (dimension == 1) {
            trivial_tour.push_back(0); // Tournée [0] pour 1 nœud
        }
        return Tour(trivial_tour, graph_); // Créer et retourner la tournée triviale
    }

    std::vector<int> tour_nodes; // Pour stocker la séquence des nœuds visités
    std::vector<bool> visited(dimension, false); // Pour suivre quels nœuds ont été visités

    int current_node = start_node;

    // Ajouter le nœud de départ à la tournée
    tour_nodes.push_back(current_node);
    visited[current_node] = true;

    // Visiter les (dimension - 1) nœuds restants
    for (int i = 0; i < dimension - 1; ++i) {
        int min_distance = std::numeric_limits<int>::max(); // Initialiser la distance minimale à une très grande valeur
        int nearest_neighbor = -1; // Pour stocker l'indice du voisin le plus proche

        // Rechercher le voisin non visité le plus proche du nœud actuel
        for (int neighbor_node = 0; neighbor_node < dimension; ++neighbor_node) {
            // Vérifier si le nœud voisin n'a pas encore été visité
            if (!visited[neighbor_node]) {
                // Obtenir la distance entre le nœud actuel et le nœud voisin
                int distance = graph_.getDistance(current_node, neighbor_node);

                // Si cette distance est plus petite que la distance minimale trouvée jusqu'à présent
                if (distance != -1 && distance < min_distance) { // S'assurer que la distance est valide (-1 indique une erreur)
                    min_distance = distance;
                    nearest_neighbor = neighbor_node;
                }
            }
        }

        // Si un voisin le plus proche a été trouvé
        if (nearest_neighbor != -1) {
            // Ajouter le voisin le plus proche à la tournée
            tour_nodes.push_back(nearest_neighbor);
            // Marquer le voisin comme visité
            visited[nearest_neighbor] = true;
            // Le voisin devient le nouveau nœud actuel
            current_node = nearest_neighbor;
        } else {
             // Ceci ne devrait pas arriver si la logique est correcte et le graphe est complet
             std::cerr << "Erreur: Impossible de trouver un voisin non visité. Algorithme interrompu prématurément." << std::endl;
             break; // Sortir de la boucle
        }
    }

    // Tous les nœuds devraient être visités à ce stade.
    // Construire l'objet Tour à partir de la séquence trouvée
    Tour result_tour(tour_nodes, graph_);

    return result_tour;
}

// L'algorithme remplace des paires d'arêtes pour réduire la distance totale de la tournée
Tour TspSolver::OptimizationSwapEdges(const Tour& tour) const {
    Tour best_tour = tour;
    bool improved = true;
    size_t n = best_tour.getNodes().size();

    while (improved) {
        improved = false;
        for (size_t i = 0; i < n - 2; ++i) {
            for (size_t j = i + 2; j < n; ++j) {
                // Nœuds des arêtes à échanger
                int a = best_tour.getNodes()[i];
                int b = best_tour.getNodes()[i + 1];
                int c = best_tour.getNodes()[j];
                int d_index = (j == n - 1) ? 0 : j + 1;
                int d = best_tour.getNodes()[d_index];

                // Calculer la variation de distance
                int delta = graph_.getDistance(a, c) + graph_.getDistance(b, d) - graph_.getDistance(a, b) - graph_.getDistance(c, d);

                std::cout << "i = " << i << ", j = " << j << ", a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << ", delta = " << delta << std::endl;
                std::cout << "Distance avant : " << best_tour.getTotalDistance() << std::endl;

                // Si l'échange améliore la tournée
                if (delta < 0) {
                    Tour new_tour = best_tour;
                    new_tour.reverseSubsequence(i + 1, j);
                    new_tour.recalculateDistance(graph_); // Recalculer la distance
                    std::cout << "Distance après : " << new_tour.getTotalDistance() << std::endl;
                    if (new_tour.getTotalDistance() < best_tour.getTotalDistance()) {
                        best_tour = new_tour;
                        improved = true;
                        std::cout << "Amélioration trouvée !" << std::endl;
                    }
                }
            }
        }
    }
    return best_tour;
}