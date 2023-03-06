#include "Sprite.hpp"

Sprite::Sprite(float x0, float y0, float xf, float yf, GLuint texture_id)
	: m_texture_id(texture_id)
{
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	m_vertices = {
		glm::vec4(xf, y0, 1.0f, 1.0f),
		glm::vec4(xf, yf, 1.0f, 0.0f),
		glm::vec4(x0, y0, 0.0f, 1.0f),
		glm::vec4(xf, yf, 1.0f, 0.0f),
		glm::vec4(x0, yf, 0.0f, 0.0f),
		glm::vec4(x0, y0, 0.0f, 1.0f)
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(m_vao);
}

Sprite::~Sprite()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Sprite::draw()
{
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}