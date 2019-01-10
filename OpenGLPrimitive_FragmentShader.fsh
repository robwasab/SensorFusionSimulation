#version 330 core
out vec4 out_color;

in vec3 color;
in vec2 texture_location;

uniform float uniform_color_ratio;
uniform sampler2D uniform_texture;

void main()
{
    out_color = mix(texture(uniform_texture, texture_location), vec4(color, 1.0), uniform_color_ratio);
}
