#include <iostream>
#include <vector>
#include <cmath>
#include "Utils.hpp"
#include "Triangulation.hpp"
#include "Bezier.hpp"

using namespace std;

int main() {
    // glfw: initialize and configure
    // ------------------------------

    GLFWwindow* window = Utils::initGlfwWindow("Triangulation 2D d'une surface plane", GLFW_CURSOR_DISABLED);

    if(window == NULL) {
        cout << "Could not create glfw window" << endl;
        return 1;
    }

    Utils::camera = new Camera(vec3(1.0f, 0.5f, 2.0f), -90.0f, 0.0f, 60.0f, 0.08f);

    // build and compile our shader program
    // ------------------------------------
    MyShader shader("../src/shaders/Vertex.vs", "../src/shaders/Fragment.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<float> controlPoints = {
        0.0f, 0.2f,
        1.0f, 0.7f,
        2.0f, 0.6f,
        1.5f, 0.2f,
        0.5f, 0.0f,
    };

    // Creer la courbe de bezier avec 10 segments par quadratique
    vector<float> courbe = Bezier::bezierParametric(controlPoints, 10);

    vector<Point> courbePoints;

    for(int i = 0; i < courbe.size() - 1; i++) {
        Point p = {courbe[i], courbe[i+1]};
        courbePoints.push_back(p);
    }

    vector<GLuint> indices = earClippingIndices(courbePoints);

    unsigned int VBO, VAO, EBO, VBO_control_points, VAO_control_points;

    // Create VAOs, VBOs and EBO
    Utils::createBuffer(courbe, indices, _2D, &VAO, &VBO, &EBO);
    Utils::createBuffer(controlPoints, _2D, &VAO_control_points, &VBO_control_points);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    shader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        Utils::camera->updateDeltaTime();

        // input
        // -----
        Utils::processInput(window);

        // render
        // ------
        // clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // pass projection matrix to MyShader (note that in this case it could change every frame)
        Utils::camera->updateView(shader, (float)Utils::SCR_WIDTH, (float)Utils::SCR_HEIGHT);

        // Afficher en bleu les points de controle
        shader.setColor(BLUE);
        glBindVertexArray(VAO_control_points);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, controlPoints.size() / 2);

        // Afficher en blanc les lignes reliant les points de controle
        shader.setColor(WHITE);
        glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size() / 2);

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        shader.setMat4("model", model);

        // Afficher en rouge tous les triangles représentant la surface
        glBindVertexArray(VAO); 
        shader.setColor(RED);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size() + 1, GL_UNSIGNED_INT, 0);

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
