#pragma once
#include <memory>

#include "Agents.hpp"

class Window;
class Shader;
class ComputeShader;
class Texture;
class Sprite;

class PhysarumSimulation
{
public:
	PhysarumSimulation(int win_width, int win_height, int swapInterval, bool isFullscreen = false);
	~PhysarumSimulation();

	void run();

	const size_t NUM_AGENTS;

	AgentSystem m_agentSystem;

	std::unique_ptr<Window> m_window;

	std::unique_ptr <Shader> m_program;
	std::unique_ptr <ComputeShader> m_agentComputeProgram;
	std::unique_ptr <ComputeShader> m_textureComputeProgram;

	std::unique_ptr <Texture> m_initialTexture;
	std::unique_ptr <Texture> m_processedTexture;
	std::unique_ptr <Sprite> m_quad;
};
