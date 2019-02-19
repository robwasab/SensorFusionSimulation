#version 330 core
layout (location = 0) in vec2 point; // pos x, pos y

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(point.xy, 0.0, 1.0);
}
