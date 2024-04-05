#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;


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

        // Calculer et ajouter le point intermédiaire
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

 

vector<float> bezier(const vector<float>& points) {
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

        for (float t = 0.0f; t <= 1.0f; t += 0.01f) {
            
            float x = powf(1.0f - t, 2.0f) * a[0] + 2 * (1 - t) * t * b[0] + t * t * c[0];
            float y = powf(1.0f - t, 2.0f) * a[1] + 2 * (1 - t) * t * b[1] + t * t * c[1];
            courbe.push_back(x);
            courbe.push_back(y);
        }
    }

    return courbe;
}


// shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "uniform vec3 my_color;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor.rgb = my_color;\n"
    "   gl_FragColor.a = 1;\n"
    "}\n\0";

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(900, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    vector<float> control_points = {
        -0.5f, -0.8f,
        -0.25f, 0.7f,
        0.0f, -0.4f,
        0.2f, 0.9f,
        0.5f, -0.6f,
        0.75f, 0.3f,
        0.9f, -0.2f,
        0.8f, 0.5f
    };


    control_points = add_intermediate_points(control_points);

    for (int i = 0; i < control_points.size(); i++) {
        cout << control_points[i] << endl;
    }

    vector<float> vertices = bezier(control_points);

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
        processInput(window);

        // render
        // ------
        // clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        GLint color_location = glGetUniformLocation(shaderProgram, "my_color");
        float color[3] = {1.0f, 1.0f, 1.0f};
        glUniform3fv(color_location, 1, color);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO_control_points); 
        //glDrawArrays(GL_LINE_STRIP, 0, control_points.size() / 2);

        // boucler chaque point de contrôle pour dessiner ligne par ligne
        for (int i = 0; i < (control_points.size() - 2) / 2; i += 2)
        {
            // Alterner la couleur pour chaque ligne
            if (i % 2 == 0) {
                float blue[3] = {0.0f, 0.0f, 1.0f};
                glUniform3fv(color_location, 1, blue);
            }
            else {
                float green[3] = {0.0f, 1.0f, 0.0f};
                glUniform3fv(color_location, 1, green);
            }
            glUseProgram(shaderProgram);

            // Ligne entre le point de controle i et le i + 1
            glDrawArrays(GL_LINE_STRIP, i, 2);

            i--;
        }



        color_location = glGetUniformLocation(shaderProgram, "my_color");
        float color2[3] = {1.0f, 0.0f, 0.0f};
        glUniform3fv(color_location, 1, color2);
        glUseProgram(shaderProgram);
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
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
