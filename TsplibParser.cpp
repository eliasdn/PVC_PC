#include "TsplibParser.h"
#include <sstream> // Pour utiliser istringstream

// Constructeur
TsplibParser::TsplibParser(const std::string& filename) : filename_(filename) {
    // Le constructeur initialise simplement le nom du fichier.
}

// Méthode principale pour parser le fichier
bool TsplibParser::parse() {
    std::ifstream file(filename_);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename_ << std::endl;
        return false;
    }

    // 1. Lire l'en-tête
    if (!parseHeader(file)) {
        file.close();
        return false;
    }

    // Vérifier la dimension
    if (dimension_ <= 0) {
        std::cerr << "Erreur de parsing: Dimension non valide ou introuvable." << std::endl;
        file.close();
        return false;
    }

    // 2. Parser les sections en fonction du type de poids des arêtes
    if (edgeWeightType_ == "EXPLICIT") {
        if (!parseEdgeWeightSection(file)) {
            file.close();
            return false;
        }
    } else if (edgeWeightType_ == "EUC_2D" || edgeWeightType_ == "ATT") {
        if (!parseNodeCoordSection(file)) {
            file.close();
            return false;
        }
        computeDistanceMatrixFromCoords();
    } else {
        std::cerr << "Erreur de parsing: Type de poids des arêtes non supporté : " << edgeWeightType_ << std::endl;
        std::cerr << "Types supportés: EXPLICIT, EUC_2D, ATT." << std::endl;
        file.close();
        return false;
    }

    // Recherche de la fin du fichier
    std::string line;
    while (std::getline(file, line)) {
        if (line == "EOF") {
            break;
        }
    }


    file.close();
    return true;
}

// Cherche et lit une ligne commençant par un mot-clé spécifié
bool TsplibParser::readLineWithKeyword(std::ifstream& file, const std::string& keyword, std::string& value) {
    file.clear(); // Effacer les flags d'erreur précédents
    file.seekg(0); // Revenir au début du fichier pour la recherche

    std::string line;
    while (std::getline(file, line)) {
        // Supprimer les espaces en début et fin de ligne
        size_t first_char = line.find_first_not_of(" \t");
        if (first_char == std::string::npos) continue; // Ligne vide ou ne contenant que des espaces

        if (line.substr(first_char, keyword.length()) == keyword) {
            // Le mot-clé est trouvé extraire la valeur après le ':' ou les espaces
            size_t separator_pos = line.find(':');
            if (separator_pos != std::string::npos) {
                value = line.substr(separator_pos + 1);
            } else {
                 // Si pas de ':', prendre tout ce qui suit le mot-clé et les espaces
                 separator_pos = line.find_first_not_of(" \t", first_char + keyword.length());
                 if (separator_pos != std::string::npos) {
                     value = line.substr(separator_pos);
                 } else {
                     value = "";
                 }
            }
            // Supprimer les espaces en début et fin de la valeur extraite
            size_t val_first_char = value.find_first_not_of(" \t");
            size_t val_last_char = value.find_last_not_of(" \t");
            if (val_first_char != std::string::npos) {
                value = value.substr(val_first_char, val_last_char - val_first_char + 1);
            } else {
                value = "";
            }

            return true;
        }
    }
    return false;
}


// Parse les informations d'en-tête du fichier
bool TsplibParser::parseHeader(std::ifstream& file) {
    std::string value;

    // Lire la dimension
    if (!readLineWithKeyword(file, "DIMENSION", value)) {
        std::cerr << "Erreur de parsing: Mot-clé DIMENSION non trouvé." << std::endl;
        return false;
    }
    try {
        dimension_ = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Erreur de parsing: Valeur de DIMENSION invalide : " << value << std::endl;
        return false;
    } catch (const std::out_of_range& oor) {
         std::cerr << "Erreur de parsing: Valeur de DIMENSION hors limites : " << value << std::endl;
         return false;
    }


    // Lire le type de poids des arêtes
    if (!readLineWithKeyword(file, "EDGE_WEIGHT_TYPE", value)) {
        std::cerr << "Erreur de parsing: Mot-clé EDGE_WEIGHT_TYPE non trouvé." << std::endl;
        return false;
    }
    edgeWeightType_ = value;


    return true;
}

// Parse la section NODE_COORD_SECTION
bool TsplibParser::parseNodeCoordSection(std::ifstream& file) {
    file.clear(); // Effacer les flags d'erreur
    file.seekg(0); // Revenir au début pour chercher la section

    std::string line;
    bool section_found = false;
    while (std::getline(file, line)) {
        if (line == "NODE_COORD_SECTION") {
            section_found = true;
            break;
        }
    }

    if (!section_found) {
        std::cerr << "Erreur de parsing: Section NODE_COORD_SECTION non trouvée." << std::endl;
        return false;
    }

    // Lire les coordonnées des nœuds
    nodeCoords_.resize(dimension_); // Allouer de l'espace pour les coordonnées
    for (int i = 0; i < dimension_; ++i) {
        if (!std::getline(file, line)) {
            std::cerr << "Erreur de parsing: Fin de fichier inattendue lors de la lecture des coordonnées du nœud " << i + 1 << std::endl;
            return false;
        }
        std::stringstream ss(line);
        int node_id;
        // Les IDs des nœuds dans le fichier commencent à 1, notre vecteur à 0
        if (!(ss >> node_id >> nodeCoords_[i].x >> nodeCoords_[i].y)) {
             std::cerr << "Erreur de parsing: Format de ligne invalide pour les coordonnées du nœud " << i + 1 << ": " << line << std::endl;
             return false;
        }
    }

    return true;
}

// Parse la section EDGE_WEIGHT_SECTION (format UPPER_ROW)
bool TsplibParser::parseEdgeWeightSection(std::ifstream& file) {
    file.clear(); // Effacer les flags d'erreur
    file.seekg(0); // Revenir au début pour chercher la section

    std::string line;
    bool section_found = false;
    while (std::getline(file, line)) {
        if (line == "EDGE_WEIGHT_SECTION") {
            section_found = true;
            break;
        }
    }

    if (!section_found) {
        std::cerr << "Erreur de parsing: Section EDGE_WEIGHT_SECTION non trouvée." << std::endl;
        return false;
    }

    // Initialiser la matrice de distances
    distanceMatrix_.resize(dimension_, std::vector<int>(dimension_));

    // Lire les distances (format UPPER_ROW)
    // Le format UPPER_ROW liste les distances du triangle supérieur de la matrice
    // ligne par ligne. Pour une matrice N x N, la ligne i (0-indexée) contient
    // les distances entre le nœud i et les nœuds i+1, i+2, ..., N-1.
    // Nombre total de distances = N * (N - 1) / 2

    std::stringstream ss;
    std::string current_line;
    int value;
    int count = 0; // Compteur pour suivre le nombre de distances lues

    // Remplir la matrice
    for (int i = 0; i < dimension_; ++i) {
        distanceMatrix_[i][i] = 0; // Distance d'un nœud à lui-même est 0
        for (int j = i + 1; j < dimension_; ++j) {
            // Lire la prochaine valeur disponible
            while (!(ss >> value)) {
                if (!std::getline(file, current_line)) {
                    std::cerr << "Erreur de parsing: Fin de fichier inattendue ou format incorrect dans EDGE_WEIGHT_SECTION." << std::endl;
                    return false;
                }
                ss.clear(); // Effacer les flags d'erreur du stringstream
                ss.str(current_line); // Charger la nouvelle ligne dans le stringstream
            }
            // Stocker la distance (la matrice est symétrique)
            distanceMatrix_[i][j] = value;
            distanceMatrix_[j][i] = value;
            count++;
        }
    }

    return true;
}

// Calcule la distance pour EDGE_WEIGHT_TYPE = ATT
int TsplibParser::calculateAttDistance(const Point& p1, const Point& p2) const {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dist = std::sqrt((dx * dx + dy * dy) / 10.0); // documentation page 6&7
    return static_cast<int>(std::round(dist));
}

// Calcule la distance euclidienne 2D (arrondie)
int TsplibParser::calculateEuclideanDistance(const Point& p1, const Point& p2) const {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dist = std::sqrt(dx * dx + dy * dy);
    return static_cast<int>(std::round(dist));
}


// Calcule la matrice des distances à partir des coordonnées
void TsplibParser::computeDistanceMatrixFromCoords() {
    if (nodeCoords_.empty() || dimension_ <= 0) {
        std::cerr << "Erreur: Coordonnées des nœuds non disponibles ou dimension invalide pour calculer la matrice de distances." << std::endl;
        return;
    }

    distanceMatrix_.resize(dimension_, std::vector<int>(dimension_));

    for (int i = 0; i < dimension_; ++i) {
        distanceMatrix_[i][i] = 0;
        for (int j = i + 1; j < dimension_; ++j) {
            int distance;
            if (edgeWeightType_ == "EUC_2D") {
                distance = calculateEuclideanDistance(nodeCoords_[i], nodeCoords_[j]);
            } else if (edgeWeightType_ == "ATT") {
                distance = calculateAttDistance(nodeCoords_[i], nodeCoords_[j]);
            distanceMatrix_[i][j] = distance;
            distanceMatrix_[j][i] = distance;
            }
        }
    }
}