#include "ComputeShader.hpp"

ComputeShader::ComputeShader(const GLchar* computePath)
{
	std::string computeCode;
	std::ifstream cshFile;

	cshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		cshFile.open(computePath);
		std::stringstream cshStream;
		cshStream << cshFile.rdbuf();
		cshFile.close();
		computeCode = cshStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Couldn't read compute shader file.\n";
	}
	const char* cshCode = computeCode.c_str();

	unsigned int compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cshCode, nullptr);
	glCompileShader(compute);
	checkCompileErrors(compute, "COMPUTE");
	id = glCreateProgram();
	glAttachShader(id, compute);
	glDeleteShader(compute);

	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");
}

void ComputeShader::dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
{
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void ComputeShader::useShaderStorageBuffer(GLsizeiptr size, void* data)
{
	glGenBuffers(1, &m_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}