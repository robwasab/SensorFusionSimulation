#version 330 core
in vec2 texture_coords;
out vec4 color;

uniform sampler2D   texture_fontface;
uniform vec3        texture_color;

void main()
{
    color = vec4(texture_color, texture(texture_fontface, texture_coords).r);
}
