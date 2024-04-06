#include "../includes/learnopengl/shader_m.h"

class MyShader : public Shader {
    
    public :

        MyShader(const char *vertexPath, const char *fragmentPath) : Shader(vertexPath, fragmentPath) {};

        void setColor(float r, float g, float b) {
            float color[3] = {r, g, b};
            this->setColor(color);
        }   

        void setColor(float color[3]) {
            GLint color_location = glGetUniformLocation(this->ID, "my_color");
            glUniform3fv(color_location, 1, color);
            this->use();
        }   
};