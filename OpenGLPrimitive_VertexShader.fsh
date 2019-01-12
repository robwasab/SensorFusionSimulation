#version 330 core
layout (location = 0) in vec3 attr_position;
layout (location = 1) in vec3 attr_color;
layout (location = 2) in vec2 attr_texture;

out vec3 color;
out vec2 texture_location;

uniform mat4 transform;
uniform float uniform_color_scale;

void main()
{
    gl_Position = transform * vec4(attr_position, 1.0);
    
    color = uniform_color_scale * attr_color;
    texture_location = attr_texture;
}
