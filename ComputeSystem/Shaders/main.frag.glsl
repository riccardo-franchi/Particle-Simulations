#version 460 core

in vec2 coords;

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture1, coords);
}