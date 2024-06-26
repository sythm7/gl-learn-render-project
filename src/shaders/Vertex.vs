#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main()
{
   gl_PointSize = 10.0;
   gl_Position = projection * view * model * vec4(aPos, 1.0);
}