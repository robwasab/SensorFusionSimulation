#version 330 core
layout (location = 0) in vec4 vertex; // pos x, pos y, texture x, texture y
out vec2 texture_coords;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vertex.xy, 0.0, 1.0);
    texture_coords = vertex.zw;
}
