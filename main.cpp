#include <iostream>
#include <string>
#include <vector>
#include <fstream> // Pour l'écriture de fichiers
#include <limits>  // Pour std::numeric_limits

#include "TsplibParser.h"
#include "Graph.h"
#include "TspSolver.h"
#include "Tour.h"

int main(int argc, char* argv[]) {
    // Vérifier le nombre d'arguments
    if (argc < 2 || argc > 3) {
        std::cerr << "Utilisation: " << argv[0] << " <chemin_vers_fichier_tsplib> [--debug]" << std::endl;
        return 1; // Quitter avec un code d'erreur
    }

    // Récupérer le nom du fichier depuis les arguments
    std::string filename = argv[1];

    // Vérifier si l'option --debug est présente
    bool debug_mode = false;
    if (argc == 3) {
        std::string debug_arg = argv[2];
        if (debug_arg == "--debug") {
            debug_mode = true;
            std::cout << "Mode debug activé. L'affichage de la matrice de distances peut être volumineux." << std::endl;
        } else {
            std::cerr << "Argument inconnu : " << debug_arg << std::endl;
            std::cerr << "Utilisation: " << argv[0] << " <chemin_vers_fichier_tsplib> [--debug]" << std::endl;
            return 1; // Quitter si l'argument inconnu
        }
    }


    // 1. Parser le fichier TSPLIB
    TsplibParser parser(filename);

    std::cout << "Tentative de parsing du fichier : " << filename << std::endl;

    if (!parser.parse()) {
        std::cerr << "Erreur lors du parsing du fichier TSPLIB. Quitting." << std::endl;
        return 1; // Quitter si le parsing échoue
    }

    std::cout << "Parsing réussi !" << std::endl;

    // 2. Créer un objet Graph à partir des données parsées
    int dimension = parser.getDimension();
    const auto& distanceMatrix = parser.getDistanceMatrix();

    if (dimension <= 0 || distanceMatrix.empty()) {
        std::cerr << "Erreur: Les données du graphe ne sont pas valides après parsing." << std::endl;
        return 1;
    }

    Graph graph(dimension, distanceMatrix);

    std::cout << "Graphe créé avec " << graph.getDimension() << " nœuds." << std::endl;

    // 3. Afficher la matrice de distances si le mode debug est activé
    if (debug_mode) {
         graph.printDistanceMatrix();
    }

    // 4. Résoudre le TSP en utilisant le TspSolver
    std::cout << std::endl << "Résolution du TSP avec l'algorithme du plus proche voisin..." << std::endl;
    TspSolver solver(graph);
    Tour solution_tour = solver.solve();

    std::cout << "Résolution terminée." << std::endl;

    // 5. Afficher la solution trouvée
    std::cout << "Solution trouvée :" << std::endl;
    solution_tour.print();

    // 6. Sauvegarder la solution dans un fichier
    std::string output_filename = filename + ".tour";

    std::ofstream outfile(output_filename);
    if (!outfile.is_open()) {
        std::cerr << "Erreur: Impossible de créer ou d'ouvrir le fichier de sortie " << output_filename << std::endl;
        // Le programme peut continuer, mais la solution ne sera pas sauvegardée
    } else {
        // Écrire la solution dans le fichier
        // Format simple : distance totale, puis séquence des nœuds
        outfile << "TOUR_DISTANCE : " << solution_tour.getTotalDistance() << std::endl;
        outfile << "TOUR_NODES :" << std::endl;
        const auto& nodes = solution_tour.getNodes();
        for (size_t i = 0; i < nodes.size(); ++i) {
            outfile << nodes[i] + 1 << std::endl;
        }
        outfile << -1 << std::endl; // Marqueur de fin de séquence (convention TSPLIB)

        outfile.close();
        std::cout << "Solution sauvegardée dans " << output_filename << std::endl;
    }


    return 0; // Quitter avec succès
}