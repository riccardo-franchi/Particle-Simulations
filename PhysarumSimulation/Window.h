#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Agents.hpp"

class Shader;
class ComputeShader;
class Texture;
class Sprite;

class Window
{
public:
	Window(int width, int height, std::string name, int swapInterval, bool isFullscreen = false);
	~Window();

	void run();

private:
	void processInput();

	GLFWwindow* m_win;

	int m_winWidth, m_winHeight;

	float m_deltaTime = 0.0f, m_prevFrame = 0.0f;

	AgentSystem m_agentSystem;

	Shader* m_program;
	ComputeShader* m_agentComputeProgram;
	ComputeShader* m_textureComputeProgram;

	Texture* m_initialTexture;
	Texture* m_processedTexture;
	Sprite* m_quad;
};
