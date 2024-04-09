#include <glm/glm.hpp>
#include "Utils.hpp"

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {

    GLFWwindow* window = Utils::initGlfwWindow("Polygone de controle en 3D", GLFW_CURSOR_DISABLED);

    if(window == NULL) {
        cout << "Could not create glfw window" << endl;
        return 1;
    }

    // build and compile our MyShader program
    MyShader shader("../src/shaders/Vertex.vs", "../src/shaders/Fragment.fs");
    Utils::camera = new Camera(vec3(1.0f, 1.5f, 3.0f), -110.0f, -20.0f, 60.0f, 0.08f);

    vector<float> controlPoints = {
        1.0f, 0.0f, 0.5f,
        0.5f, 1.0f, 0.5f,

        1.0f, 0.0f, -0.5f,
        0.5f, 1.0f, -0.5f,

        -1.0f, 0.0f, 0.5f,
        -0.5f, 1.0f, 0.5f,

        -1.0f, 0.0f, -0.5f,
        -0.5f, 1.0f, -0.5f,
    };

    vector<GLuint> indices = {
        0, 1, 2,
        1, 2, 3,

        1, 3, 5,
        3, 5, 7,

        4, 5, 6,
        5, 6, 7
    };

    vector<float> axis3D = {
        0, 0, 0,
        5, 0, 0,

        0, 0, 0,
        0, 5, 0,
        
        0, 0, 0,
        0, 0, 5
    };

    unsigned int VBO, VAO, VBO_control_points, VAO_control_points, EBO;

    Utils::createBuffer(controlPoints, indices, _3D, &VAO, &VBO, &EBO);
    Utils::createBuffer(axis3D, _3D, &VAO_control_points, &VBO_control_points);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    // render loop
    // -----------

    shader.use();

    while (!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        Utils::camera->updateDeltaTime();

        // input
        // -----
        Utils::processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // pass projection matrix to MyShader (note that in this case it could change every frame)
        Utils::camera->updateView(shader, (float)Utils::SCR_WIDTH, (float)Utils::SCR_HEIGHT);

        // render boxes
        glBindVertexArray(VAO_control_points);
        // calculate the model matrix for each object and pass it to MyShader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        shader.setMat4("model", model);

        // Afficher un axe 3D permettant de se repérer dans l'espace
        shader.setColor(BLUE);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        shader.setColor(GREEN);
        glDrawArrays(GL_LINE_STRIP, 2, 4);
        shader.setColor(RED);
        glDrawArrays(GL_LINE_STRIP, 4, 6);

        // Afficher le polygone de controle en 3D
        shader.setColor(PURPLE);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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