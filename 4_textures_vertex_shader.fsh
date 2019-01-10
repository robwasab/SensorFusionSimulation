#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec2 texture_coord;
out vec3 color;

uniform vec3 offset;

void main()
{
    vec3 pos = aPos + offset;
    gl_Position = vec4(pos, 1.0);
    
    color = aColor;
    texture_coord = aTexture;
}
