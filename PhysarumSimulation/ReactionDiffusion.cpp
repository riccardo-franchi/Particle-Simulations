#include "ReactionDiffusion.hpp"

#include "Window.hpp"
#include "Shader.hpp"
#include "ComputeShader.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"

const int COMPUTATIONS_PER_FRAME = 20;

ReactionDiffusion::ReactionDiffusion(int win_width, int win_height, int swapInterval, bool isFullscreen)
{
	m_window = std::make_unique<Window>(win_width, win_height, "Reaction-Diffusion Simulation", swapInterval, isFullscreen);

	m_program = std::make_unique<Shader>("Shaders/main.vert.glsl", "Shaders/main.frag.glsl");
	m_textureInitComputeProgram = std::make_unique<ComputeShader>("Shaders/Reaction-Diffusion/initTexture.comp.glsl");
	m_agentComputeProgram = std::make_unique<ComputeShader>("Shaders/Reaction-Diffusion/reactionMap.comp.glsl");
	m_textureComputeProgram = std::make_unique<ComputeShader>("Shaders/Reaction-Diffusion/outputMap.comp.glsl");

	m_initialTexture   = std::make_unique<Texture>(win_width, win_height, 0, GL_READ_WRITE);
	m_processedTexture = std::make_unique<Texture>(win_width, win_height, 1, GL_WRITE_ONLY);

	m_quad = std::make_unique<Sprite>(-1.0f, 1.0f, 1.0f, -1.0f, m_processedTexture->id);
}

ReactionDiffusion::~ReactionDiffusion()
{
}

void ReactionDiffusion::run()
{
	GLuint groups_x = m_window->getWidth() / 8;
	GLuint groups_y = m_window->getHeight() / 8;

	m_textureInitComputeProgram->use();
	m_textureInitComputeProgram->dispatch(groups_x, groups_y, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	while (!m_window->windowShouldClose())
	{
		m_window->use();

		m_agentComputeProgram->use();
		for (int i = 0; i < COMPUTATIONS_PER_FRAME; i++)
		{
			m_agentComputeProgram->dispatch(groups_x, groups_y, 1);
		}

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		m_textureComputeProgram->use();
		m_textureComputeProgram->setInt("displayMode", static_cast<int>(m_displayMode));
		m_textureComputeProgram->dispatch(groups_x, groups_y, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_program->use();
		m_quad->draw();

		if (glfwGetKey(m_window->getWindowPtr(), GLFW_KEY_1) == GLFW_PRESS)
		{ m_displayMode = DisplayMode::AB; }
		else if (glfwGetKey(m_window->getWindowPtr(), GLFW_KEY_2) == GLFW_PRESS)
		{ m_displayMode = DisplayMode::GREYSCALE; }
		else if (glfwGetKey(m_window->getWindowPtr(), GLFW_KEY_3) == GLFW_PRESS)
		{ m_displayMode = DisplayMode::DELTA1; }
		else if (glfwGetKey(m_window->getWindowPtr(), GLFW_KEY_4) == GLFW_PRESS)
		{ m_displayMode = DisplayMode::DELTA2; }

		m_window->unuse();
	}

}
