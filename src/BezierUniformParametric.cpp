#include <iostream>
#include <vector>
#include "Utils.hpp"
#include "Bezier.hpp"

using namespace std;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    GLFWwindow* window = Utils::initGlfwWindow("Courbe de bezier uniforme parametrique", GLFW_CURSOR_NORMAL);

    if(window == NULL) {
        cout << "Could not create glfw window" << endl;
        return 1;
    }

    // build and compile our shader program
    // ------------------------------------
    MyShader shader("../src/shaders/Vertex.vs", "../src/shaders/Fragment.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<float> controlPoints = {
        -0.8f, -0.7f,
        -0.6f, 0.5f,
        -0.4f, -0.4f,
        -0.2f, 0.3f,
        0.0f, -0.2f,
        0.2f, 0.1f,
        0.4f, -0.1f,
    };

    // Creer la courbe de bezier avec 10 segments par quadratique
    vector<float> vertices = Bezier::bezierParametric(controlPoints, 10);

    unsigned int VBO, VAO, VBO_control_points, VAO_control_points;

    // Create VAOs and VBOs
    Utils::createBuffer(vertices, _2D, &VAO, &VBO);
    Utils::createBuffer(controlPoints, _2D, &VAO_control_points, &VBO_control_points);

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
        
        // Afficher les points de controle en blanc
        shader.setColor(WHITE);
        glBindVertexArray(VAO_control_points); 
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, controlPoints.size() / 2);

        // Afficher les lignes reliant les points de controle en gris
        shader.setColor(GREY);
        glBindVertexArray(VAO_control_points); 
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size() / 2);

        // Afficher la courbe en rouge
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
    glDeleteVertexArrays(1, &VAO_control_points);
    glDeleteBuffers(1, &VBO_control_points);
    glDeleteProgram(shader.ID);
    delete Utils::camera;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}