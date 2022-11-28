#pragma once
#include <glad/glad.h>

class Texture
{
public:
	GLuint id = 0;
	Texture(int w, int h, GLuint unit, GLenum access);
	~Texture() {}
};

