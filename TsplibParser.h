#ifndef TSPLIB_PARSER_H
#define TSPLIB_PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath> // Pour sqrt et round

// Structure pour stocker les coordonnées d'un nœud
struct Point {
    double x;
    double y;
};

class TsplibParser {
public:
    // Constructeur prenant le nom du fichier TSPLIB
    TsplibParser(const std::string& filename);

    // Méthode pour lancer le parsing du fichier
    bool parse();

    // Getters pour accéder aux données parsées
    int getDimension() const { return dimension_; }
    std::string getEdgeWeightType() const { return edgeWeightType_; }
    const std::vector<Point>& getNodeCoords() const { return nodeCoords_; }
    const std::vector<std::vector<int>>& getDistanceMatrix() const { return distanceMatrix_; }

private:
    std::string filename_;
    int dimension_ = 0;
    std::string edgeWeightType_;
    std::vector<Point> nodeCoords_;
    std::vector<std::vector<int>> distanceMatrix_;

    // Méthodes privées pour le parsing 
    // Cherche et lit une ligne commençant par un mot-clé spécifié
    bool readLineWithKeyword(std::ifstream& file, const std::string& keyword, std::string& value);

    // Parse les informations d'en-tête du fichier
    bool parseHeader(std::ifstream& file);

    // Parse la section NODE_COORD_SECTION
    bool parseNodeCoordSection(std::ifstream& file);

    // Parse la section EDGE_WEIGHT_SECTION (format UPPER_ROW)
    bool parseEdgeWeightSection(std::ifstream& file);

    // Calcule la distance pour EDGE_WEIGHT_TYPE = ATT
    int calculateAttDistance(const Point& p1, const Point& p2) const;

    // Calcule la distance euclidienne 2D (arrondie)
    int calculateEuclideanDistance(const Point& p1, const Point& p2) const;

    // Calcule la matrice des distances à partir des coordonnées
    void computeDistanceMatrixFromCoords();
};

#endif