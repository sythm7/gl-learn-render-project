#include <vector>
#include <cmath>

namespace Bezier {

    std::vector<float> bezierParametric(std::vector<float>& control_points, int nb_segments);
    std::vector<float> addIntermediatePoints(std::vector<float> points);


    std::vector<float> bezierParametric(std::vector<float>& control_points, int nb_segments) {

        std::vector<float> points = addIntermediatePoints(control_points);

        std::vector<float> courbe;

        // Regarder si il y a plus de 3 points
        if (points.size() < 6) {
            return courbe;
        }

        // on calcule 3 par 3 les bézier quadratiques
        for (size_t i = 0; i < points.size() - 4; i += 4) {
            float a[2] = {points[i], points[i + 1]};
            float b[2] = {points[i + 2], points[i + 3]};
            float c[2] = {points[i + 4], points[i + 5]};

            for (float t = 0.0f; t <= 1.0f; t += 1.0f/(float)nb_segments) {
                
                float x = powf(1.0f - t, 2.0f) * a[0] + 2 * (1 - t) * t * b[0] + t * t * c[0];
                float y = powf(1.0f - t, 2.0f) * a[1] + 2 * (1 - t) * t * b[1] + t * t * c[1];
                courbe.push_back(x);
                courbe.push_back(y);
            }
        }

        return courbe;
    }



    std::vector<float> addIntermediatePoints(std::vector<float> points) {
        std::vector<float> res;

        int size = points.size();

        if (size < 4*2) {
            return points; // Si moins de 4 points de contrôle, pas besoin d'ajouter des points intermédiaires
        }

        // Ajouter le premier point
        res.push_back(points[0]);
        res.push_back(points[1]);

        for(int i = 2; i < size - 2; i+=2) {

            // Ajouter point
            res.push_back(points[i]);
            res.push_back(points[i+1]);

            // Calculer et ajouter le point intermédiaire (sauf le dernier)
            if(i < size - 4) {
                float intermediate_x = (points[i] + points[i + 2]) / 2.0f;
                float intermediate_y = (points[i+1] + points[i+3]) / 2.0f;
                res.push_back(intermediate_x);
                res.push_back(intermediate_y);
            }
            
        }

        // Ajouter le dernier point
        res.push_back(points[size - 2]);
        res.push_back(points[size - 1]);

        return res;
    }

}