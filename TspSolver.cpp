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
    return nearestNeighborSolve();
}

// Implémentation de l'algorithme du plus proche voisin
Tour TspSolver::nearestNeighborSolve() const {
    int dimension = graph_.getDimension();

    // Vérification de base pour un graphe valide
    if (dimension <= 1) {
        // Pour 0 ou 1 nœud, le tour est triviale
        std::vector<int> trivial_tour;
        if (dimension == 1) {
            trivial_tour.push_back(0); // Tournée [0] pour 1 nœud
        }
        return Tour(trivial_tour, graph_); // Créer et retourner le tour triviale
    }

    std::vector<int> tour_nodes; // Pour stocker la séquence des nœuds visités
    std::vector<bool> visited(dimension, false); // Pour suivre quels nœuds ont été visités

    // Choisir un nœud de départ (on commence ici par le nœud 0)
    int start_node = 0;
    int current_node = start_node;

    // Ajouter le nœud de départ au tour
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
            // Ajouter le voisin le plus proche au tour
            tour_nodes.push_back(nearest_neighbor);
            // Marquer le voisin comme visité
            visited[nearest_neighbor] = true;
            // Le voisin devient le nouveau nœud actuel
            current_node = nearest_neighbor;
        } else {
             // Ceci ne devrait pas arriver si la logique est correcte et le graphe est complet,
             // mais c'est bien de gérer ce cas pour robustesse.
             std::cerr << "Erreur: Impossible de trouver un voisin non visité. Algorithme interrompu prématurément." << std::endl;
             break; // Sortir de la boucle
        }
    }

    // Tous les nœuds devraient être visités à ce stade.
    // Construire l'objet Tour à partir de la séquence trouvée
    Tour result_tour(tour_nodes, graph_);

    return result_tour; // Retourner le tour construite
}