#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
// 
//----------Window_Size----------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//------------------------------------

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"uniform vec3 color;\n"
"void main()\n"
"{\n"
"   gl_FragColor.rgb = color;\n"
"   gl_FragColor.a = 1;\n"
"}\n\0";

// Définition d'une structure pour représenter un point dans l'espace 2D
struct Point {
    float x;
    float y;
};


vector<float> add_intermediate_points(vector<float> points) {
    vector<float> res;

    int size = points.size();

    if (size < 4 * 2) {
        return points; // Si moins de 4 points de contrôle, pas besoin d'ajouter des points intermédiaires
    }

    // Ajouter le premier point
    res.push_back(points[0]);
    res.push_back(points[1]);

    for (int i = 2; i < size - 2; i += 2) {

        // Ajouter point
        res.push_back(points[i]);
        res.push_back(points[i + 1]);

        // Calculer et ajouter le point intermédiaire (sauf le dernier)
        if (i < size - 4) {
            float intermediate_x = (points[i] + points[i + 2]) / 2.0f;
            float intermediate_y = (points[i + 1] + points[i + 3]) / 2.0f;
            res.push_back(intermediate_x);
            res.push_back(intermediate_y);
        }

    }

    // Ajouter le dernier point
    res.push_back(points[size - 2]);
    res.push_back(points[size - 1]);

    return res;
}



vector<float> bezier_parametrique(const vector<float>& points, int nb_segments) {
    vector<float> courbe;

    // Regarder si il y a plus de 3 points
    if (points.size() < 6) {
        return courbe;
    }

    // on calcule 3 par 3 les bézier quadratiques
    for (size_t i = 0; i < points.size() - 4; i += 4) {
        float a[2] = { points[i], points[i + 1] };
        float b[2] = { points[i + 2], points[i + 3] };
        float c[2] = { points[i + 4], points[i + 5] };

        for (float t = 0.0f; t <= 1.0f; t += 1.0f / (float)nb_segments) {

            float x = powf(1.0f - t, 2.0f) * a[0] + 2 * (1 - t) * t * b[0] + t * t * c[0];
            float y = powf(1.0f - t, 2.0f) * a[1] + 2 * (1 - t) * t * b[1] + t * t * c[1];
            courbe.push_back(x);
            courbe.push_back(y);
            cout << "Point at t=" << t << ": (" << x << ", " << y << ")" << endl;
        }
    }

    return courbe;
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Courbes Beziers", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    vector<float> test;


    //  Programme
    //--------------------------------------------------------------------------------

    // données du vertex

    
    /*vector<Point> controlPoints = {
       {-0.5f, -0.5f},
       {-0.4f, -0.3f},
       {-0.2f, -0.1f},
       {0.0f, 0.0f},
       {0.0f, -0.1f},
       {0.0f, 0.1f},
       {0.4f, -0.1f}
    };*/



    vector<float> control_points = {
        -0.8f, -0.7f,
        -0.6f, 0.5f,
        -0.4f, -0.4f,
        -0.2f, 0.3f,
        0.0f, -0.2f,
        0.2f, 0.1f,
        0.4f, -0.1f,
    };


    control_points = add_intermediate_points(control_points);

    test = bezier_parametrique(control_points, 5);


    /*vector<float> bufferData;
    for (const Point& point : control_points) {
        bufferData.push_back(point.x);
        bufferData.push_back(point.y);
    }*/




    // Évaluation de la courbe de Bézier à différents paramètres t
    /*for (float t = 0.0f; t <= 1.0f; t += 0.2f) {
        Point pointOnCurve = DeCasteljau(controlPoints, t);
        test.push_back(pointOnCurve.x);
        test.push_back(pointOnCurve.y);
        cout << "Point at t=" << t << ": (" << pointOnCurve.x << ", " << pointOnCurve.y << ")" << endl;
    }*/


    // compilation des Shaders
    // ---------------------------------
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    // link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    // deleting the Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // GLuint VBO, VAO, VBOl, VAOl;


    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);

    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, test.size() * sizeof(float), test.data(), GL_STATIC_DRAW);// Mettre en mémoire dans la cg

    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0); //position de notre vertex

    // glGenVertexArrays(1, &VAOl);
    // glGenBuffers(1, &VBOl);

    // glBindVertexArray(VAOl);

    // glBindBuffer(GL_ARRAY_BUFFER, VBOl);
    // glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), bufferData.data(), GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0); //position de notre vertex

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    unsigned int VBO, VAO, VBOl, VAOl;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, test.size() * sizeof(float), test.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // VBO & VAO for control points
    glGenVertexArrays(1, &VAOl);
    glGenBuffers(1, &VBOl);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOl);

    glBindBuffer(GL_ARRAY_BUFFER, VBOl);
    glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(float), control_points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //--------------------------------------------------------------------------------


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);


        // rendering commands here


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLint color_location = glGetUniformLocation(shaderProgram, "color");
        float color1[3] = { 1.0f, 1.0f, 1.0f };
        glUniform3fv(color_location, 1, color1);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOl);
        glDrawArrays(GL_LINE_STRIP, 0, control_points.size() / 2);


        color_location = glGetUniformLocation(shaderProgram, "color");
        float color2[3] = { 0.45f, 0.85f, 0.5f };
        glUniform3fv(color_location, 1, color2);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, test.size()/2);

        



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Delete window before ending the programm
    glfwDestroyWindow(window);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}