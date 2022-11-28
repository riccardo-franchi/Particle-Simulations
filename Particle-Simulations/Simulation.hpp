#pragma once
#include <memory>
#include <glad/glad.h>

class Window;
class Shader;
class ComputeShader;
class Texture;
class Sprite;

class Simulation
{
public:
	virtual void run() = 0;
protected:

	std::unique_ptr<Window> m_window;

	std::unique_ptr<Shader> m_program;
	std::unique_ptr<ComputeShader> m_agentComputeProgram;
	std::unique_ptr<ComputeShader> m_textureComputeProgram;

	std::unique_ptr<Texture> m_initialTexture;
	std::unique_ptr<Texture> m_processedTexture;
	std::unique_ptr<Sprite> m_quad;
};

