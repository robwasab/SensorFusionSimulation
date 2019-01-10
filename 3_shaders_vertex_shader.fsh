#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor; // output a color to the fragment shader
uniform vec3 offset;

void main()
{
    vec3 pos = aPos + offset;
    gl_Position = vec4(pos, 1.0);
    vertexColor = aColor;
}
