#version 330 core
out vec4 out_color;

in vec3 color;
in vec2 texture_coord;

uniform sampler2D texture_one;
uniform sampler2D texture_two;

void main()
{
    out_color =
    mix(texture(texture_one, texture_coord), texture(texture_two, texture_coord), 0.5);
}
