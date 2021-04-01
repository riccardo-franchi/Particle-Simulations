#pragma once
#include "Shader.h"

class ComputeShader : public Shader
{
public:
	ComputeShader() {}
	ComputeShader(const GLchar* computePath);

	void dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);

	void useShaderStorageBuffer(GLsizeiptr size, void* data);

private:
	GLuint m_ssbo = 0;
};
