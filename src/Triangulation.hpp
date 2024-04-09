#include <iostream>
#include <vector>

//#define PRINT_DEBUG 1

struct Edge {
    int previous;
    int current;
    int next;

    Edge(int previous, int current, int next) : previous(previous), current(current), next(next) {}
};

// Structure représentant un point 2D
struct Point {
    float x, y;
};

// Fonction pour calculer le produit scalaire entre deux vecteurs
float scalarProduct(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

// Fonction pour calculer la norm d'un vecteur
float norm(const Point& p) {
    return std::sqrt(p.x * p.x + p.y * p.y);
}

// Fonction pour calculer le cosinus de l'angle entre deux vecteurs
float cosinus(const Point& a, const Point& b) {
    return scalarProduct(a, b) / (norm(a) * norm(b));
}

// Fonction pour vérifier si l'angle formé par trois points est convexe
bool isConvex(const Point& a, const Point& b, const Point& c) {
    Point ab = {b.x - a.x, b.y - a.y};
    Point bc = {c.x - b.x, c.y - b.y};
    float cosTheta = cosinus(ab, bc);
    return cosTheta < 0; // Si le cosinus est négatif, l'angle est convexe
}

// Fonction pour vérifier si un point est à l'intérieur d'un triangle
bool isInsideTriangle(const Point& p, const Point& a, const Point& b, const Point& c) {
    float s1 = (p.x - a.x) * (b.y - a.y) - (b.x - a.x) * (p.y - a.y);
    float s2 = (p.x - b.x) * (c.y - b.y) - (c.x - b.x) * (p.y - b.y);
    float s3 = (p.x - c.x) * (a.y - c.y) - (a.x - c.x) * (p.y - c.y);
    return (s1 >= 0 && s2 >= 0 && s3 >= 0) || (s1 <= 0 && s2 <= 0 && s3 <= 0);
}

// Algorithme d'ear-clipping pour trianguler un polygone de controle
std::vector<unsigned int> earClippingIndices(const std::vector<Point>& points) {
    std::vector<unsigned int> indices;

    std::vector<Edge> remainingPoints;

    // On crée des arêtes reliant les indices précédents et suivants de tous les sommets
    for(int i = 0; i < points.size(); i++) {
        
        int previous = i - 1;

        if(i == 0) {
            previous = points.size() - 1;
        }

        remainingPoints.push_back(Edge(previous, i, (i+1) % points.size()));
    }

    // Tant qu'il reste 3 points ou plus, on cherche les indices des triangles
    // les 3 derniers points formeront le dernier triangle
    while (remainingPoints.size() >= 3) {

        // Booleen qui va servir a savoir si une oreille a été trouvée.
        // Une oreille est un triangle formé par 3 sommets convexes qui ne contient pas de points à l'intérieur
        bool earFound = false;

        // On cherche des oreilles en explorant tous les points en partant du premier
        for (int i = 0; i < remainingPoints.size(); i++) {

            Edge currentEdge = remainingPoints[i];

            const Point pCurrent = points[currentEdge.current];
            const Point pNext = points[currentEdge.next];
            const Point pPrevious = points[currentEdge.previous];
            
            #ifdef PRINT_DEBUG
                cout << "-------------------------------------------------" << endl;
                cout << "Point selectionne :\n\t" << "prev : " << currentEdge.previous << ", current : " << currentEdge.current << ", next : " << currentEdge.next << endl << endl;
            #endif

            // Vérification de l'angle convexe entre le point courant, suivant et précédent.
            if (isConvex(pCurrent, pNext, pPrevious)) { 
                #ifdef PRINT_DEBUG
                    cout << "L'angle est convexe" << endl;
                #endif

                // On dit pour l'instant que notre oreille est valide puis on explore tous les points restants afin de savoir si ils pourraient potentiellement être contenus dans le triangle.
                bool isEar = true;
                for (int j = 0; j < remainingPoints.size(); j++) {
                    int currentJ = remainingPoints[j].current;
                    if (isInsideTriangle(points[currentJ], pCurrent, pNext, pPrevious) && currentJ != currentEdge.current && currentJ != currentEdge.next && currentJ != currentEdge.previous) {
                        #ifdef PRINT_DEBUG
                            cout << remainingPoints[j].current << " est dans le triangle donc ce n'est pas une oreille" << endl;
                        #endif
                        // Dans ce cas, il y a un point dans le triangle, donc l'oreille n'est plus valide.
                        isEar = false;
                        break;
                    }
                    else {
                        #ifdef PRINT_DEBUG
                            cout << remainingPoints[j].current << " n'est pas dans le triangle" << endl;
                        #endif
                    }
                }

                // Si l'oreille est encore valide, on ajoute les indices du triangle dans la liste des indices.
                if (isEar) {
                    #ifdef PRINT_DEBUG
                        cout << endl << "-> Aucun point n'est dans le triangle donc c'est une oreille" << endl << endl;
                        cout << "On ajoute " << currentEdge.previous << ", " << currentEdge.current << " et " << currentEdge.next << " aux indices." << endl;
                    #endif
                    // Ajout des indices du triangle
                    indices.push_back(currentEdge.previous);
                    indices.push_back(currentEdge.current);
                    indices.push_back(currentEdge.next);

                    int prev = i == 0 ? remainingPoints.size() - 1 : i - 1;

                    // On relie le point précédent du point courant au point précédent du point suivant
                    // puis on relie le point suivant du point courant au point suivant du point précédent.
                    remainingPoints[(i + 1) % remainingPoints.size()].previous = currentEdge.previous;
                    remainingPoints[prev].next = currentEdge.next;

                    #ifdef PRINT_DEBUG
                        cout << "On supprime " << currentEdge.current << " de la liste." << endl;
                        cout << "Le precedent de " << remainingPoints[(i + 1) % remainingPoints.size()].current << " devient " << currentEdge.previous << endl;
                        cout << "Le suivant de " << remainingPoints[prev].current << " devient " << currentEdge.next << endl;
                        cout << "-------------------------------------------------" << endl << endl;
                    #endif

                    // On peut maintenant supprimer le point courant car ses points précédent et suivant sont reliés.
                    remainingPoints.erase(remainingPoints.begin() + i);
                    // On dit que l'on a trouvé une oreille puis on cherche les oreilles suivantes
                    earFound = true;
                    break;
                }
            }
        }

        if (!earFound) {
            break; // Si aucune oreille n'a été trouvée, on arrête l'algorithme
        }
    }

    return indices;
}