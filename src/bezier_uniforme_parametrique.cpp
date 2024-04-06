#include <iostream>
#include <vector>
#include <cmath>
#include "Utils.hpp"

using namespace std;

#define PAINT_CONTROL_POINTS 0

vector<float> bezier_parametrique(vector<float>& points, int nb_segments);
vector<float> add_intermediate_points(vector<float> points);

int main() {
    // glfw: initialize and configure
    // ------------------------------
    GLFWwindow* window = Utils::init_glfw_window("Courbe de bezier uniforme parametrique");

    if(window == NULL) {
        cout << "Could not create glfw window" << endl;
        return 1;
    }

    // build and compile our shader program
    // ------------------------------------
    MyShader shader("../src/shaders/bezier_surface.vs", "../src/shaders/bezier_surface.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<float> control_points = {
        -0.8f, -0.7f,
        -0.6f, 0.5f,
        -0.4f, -0.4f,
        -0.2f, 0.3f,
        0.0f, -0.2f,
        0.2f, 0.1f,
        0.4f, -0.1f,
    };

    vector<float> vertices = bezier_parametrique(control_points, 10);

    unsigned int VBO, VAO, VBO_control_points, VAO_control_points;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // VBO & VAO for control points
    glGenVertexArrays(1, &VAO_control_points);
    glGenBuffers(1, &VBO_control_points);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO_control_points);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_control_points);
    glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(float), control_points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        Utils::processInput(window);

        // render
        // ------
        // clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ICI COLOR
        glBindVertexArray(VAO_control_points); 
        
        shader.setColor(WHITE);

        // boucler chaque point de contrôle pour dessiner ligne par ligne
        if(PAINT_CONTROL_POINTS) {
            for (int i = 0; i < (control_points.size() - 2) / 2; i += 2)
                {
                    // Alterner la couleur pour chaque ligne
                    if (i % 2 == 0) {
                        shader.setColor(BLUE);
                    }
                    else {
                        shader.setColor(GREEN);
                    }

                    // Ligne entre le point de controle i et le i + 1
                    glDrawArrays(GL_LINE_STRIP, i, 2);

                    i--;
                }
        }
        else {
            glDrawArrays(GL_LINE_STRIP, 0, control_points.size() / 2);
        }

        shader.setColor(RED);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2);

        glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


vector<float> bezier_parametrique(vector<float>& points, int nb_segments) {

    points = add_intermediate_points(points);

    vector<float> courbe;

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



vector<float> add_intermediate_points(vector<float> points) {
    vector<float> res;

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