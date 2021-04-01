#version 460 core

layout (location = 0) in vec2 uvCoords;
layout (location = 1) in vec2 textureCoords;
out vec2 coords;

void main()
{
    coords = textureCoords;
    gl_Position = vec4(uvCoords, 0.0, 1.0);
}