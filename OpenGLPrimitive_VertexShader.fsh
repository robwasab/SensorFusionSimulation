#version 330 core
layout (location = 0) in vec3 attr_position;
layout (location = 1) in vec3 attr_color;
layout (location = 2) in vec2 attr_texture;

out vec3 color;
out vec2 texture_location;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(attr_position, 1.0);
    
    color = attr_color;
    texture_location = attr_texture;
}
