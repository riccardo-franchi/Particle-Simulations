#pragma once

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

	AgentSystem m_agentSystem;

	Window* m_window;

	Shader* m_program;
	ComputeShader* m_agentComputeProgram;
	ComputeShader* m_textureComputeProgram;

	Texture* m_initialTexture;
	Texture* m_processedTexture;
	Sprite* m_quad;
};
