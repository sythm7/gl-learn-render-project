#version 330 core
uniform vec3 my_color = vec3(1.0, 1.0, 1.0);
void main()
{
   gl_FragColor.rgb = my_color;
   gl_FragColor.a = 1;
}