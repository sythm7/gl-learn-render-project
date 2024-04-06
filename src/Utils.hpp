#include <iostream>
#include "glad/glad.h"
#include "Camera.hpp"

#define BLACK 0.0f, 0.0f, 0.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define YELLOW 1.0f, 1.0f, 0.0f
#define ORANGE 1.0f, 0.5f, 0.0f
#define GREY 0.62f, 0.62f, 0.62f
#define PURPLE 1.0f, 0.0f, 1.0f
#define CYAN 0.0f, 1.0f, 1.0f


namespace Utils {

    Camera* camera = NULL;

    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void processInput(GLFWwindow *window);

    GLFWwindow* init_glfw_window(const char* title) {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Surface de bezier", NULL, NULL);
        if (window == NULL) {
            cout << "Failed to create GLFW window" << endl;
            glfwTerminate();
            return NULL;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            cout << "Failed to initialize GLAD" << endl;
            glfwTerminate();
            return NULL;
        }

        return window;
    }    

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {

        // don't process the event
        if(camera == NULL) {
            return;
        }

        static bool firstMouse = true;
        static float X = SCR_WIDTH / 2.0f;
        static float Y = SCR_HEIGHT / 2.0f;

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            X = xpos;
            Y = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - X;
        float yoffset = Y - ypos; // inverted since y-coordinates range from bottom to top

        X = xpos;
        Y = ypos;

        float sensitivity = camera->getSensitivity();
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        float yaw = camera->getYaw() + xoffset;
        float pitch = camera->getPitch() + yoffset;

        // Clamp pitch to prevent flipping
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        camera->setYaw(yaw);
        camera->setPitch(pitch);
        camera->updateAngle();
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {

        // don't process the event
        if(camera == NULL) {
            return;
        }

        float fov = camera->getFov();

        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 100.0f)
            fov = 100.0f;

        camera->setFov(fov);
    }

    void processInput(GLFWwindow *window)
    {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // don't continue to process the event
        if(camera == NULL) {
            return;
        }

        camera->processInput(window);
    }
}