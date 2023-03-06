#pragma once
#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Sprite
{
public:
	Sprite(float x0, float y0, float xf, float yf, GLuint texture_id);
	~Sprite();

	void draw();

private:

	GLuint m_vbo{ 0 }, m_vao{ 0 };
	GLuint m_texture_id{ 0 };
	std::array<glm::vec4, 6> m_vertices;
};
