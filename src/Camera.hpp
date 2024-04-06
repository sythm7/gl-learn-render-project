#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MyShader.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

class Camera {

    private :
        vec3 cameraPos;
        vec3 cameraFront;
        vec3 cameraUp;
        bool firstMouse = true;
        float yaw;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
        float pitch;
        float fov;
        float deltaTime;	// time between current frame and last frame
        float lastFrame;
        float sensitivity;

    public  :

        Camera(vec3 cameraPos = vec3(0.0f, 2.0f, 4.0f),
                float yaw = -90.0f,
                float pitch = -20.0f,
                float fov = 60.0f,
                float sensitivity = 0.1f) 
        {
            this->cameraPos = cameraPos;
            this->cameraFront = vec3(0.0f, 0.0f, 0.0f);
            this->cameraUp = vec3(0.0f, 1.0f, 0.0f);
            this->yaw = yaw;
            this->pitch = pitch;
            this->fov = fov;
            this->deltaTime = 0.0f;
            this->lastFrame = 0.0f;
            this->sensitivity = sensitivity;
            this->updateAngle();
        }

        void updateView(Shader& shader, float width, float height) {

            mat4 projection = perspective(radians(this->fov), width / height, 0.1f, 100.0f);
            shader.setMat4("projection", projection);

            // camera/view transformation
            mat4 view = lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
            shader.setMat4("view", view);
        }

        void updateAngle() {
            vec3 front;
            front.x = cos(radians(this->yaw)) * cos(radians(this->pitch));
            front.y = sin(radians(this->pitch));
            front.z = sin(radians(this->yaw)) * cos(radians(this->pitch));
            this->cameraFront = normalize(front);
        }

        void updateDeltaTime() {
            float currentFrame = static_cast<float>(glfwGetTime());
            this->deltaTime = currentFrame - lastFrame;
            this->lastFrame = currentFrame;
        }

        void processInput(GLFWwindow *window) {
            float cameraSpeed = static_cast<float>(2.5 * deltaTime);
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cameraPos += cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cameraPos -= cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        void setFov(float fov) {
            this->fov = fov;
        }

        void setYaw(float yaw) {
            this->yaw = yaw;
        }

        void setPitch(float pitch) {
            this->pitch = pitch;
        }

        void setFirstMouse(bool first) {
            this->firstMouse = first;
        }

        vec3 getPos() {
            return this->cameraPos;
        }

        vec3 getFront() {
            return this->cameraFront;
        }

        vec3 getUp() {
            return this->cameraUp;
        }

        float getYaw() {
            return this->yaw;
        }

        float getFov() {
            return this->fov;
        }

        float getPitch() {
            return this->pitch;
        }

        float getSensitivity() {
            return this->sensitivity;
        }

        bool isfirstMouse() {
            return this->firstMouse;
        }

};