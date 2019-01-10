#version 330 core
out vec4 FragColor;

//uniform vec4 applicationColor;
in vec3 vertexColor;

void main()
{
    //FragColor = vec4(0.5, 0.5*applicationColor.y, 0.0, 0.1);
    FragColor = vec4(vertexColor, 1.0);
}
