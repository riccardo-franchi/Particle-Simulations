#include "PhysarumSimulation.hpp"

#include <glad/glad.h>

#include "Window.hpp"
#include "Shader.hpp"
#include "ComputeShader.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"

// Ratio of particles over total pixels
const float PARTICLE_RATIO = 0.04f;

PhysarumSimulation::PhysarumSimulation(int win_width, int win_height, int swapInterval, bool isFullscreen)
	: NUM_AGENTS((size_t)(PARTICLE_RATIO * win_width * win_height))
{
	m_agentSystem = AgentSystem(win_width, win_height, NUM_AGENTS, 1, PositionMode::RANDOM);

	m_window = std::make_unique<Window>(win_width, win_height, "Physarum Simulation", swapInterval, isFullscreen);

	m_program = std::make_unique<Shader>("Shaders/main.vert.glsl", "Shaders/main.frag.glsl");
	m_agentComputeProgram = std::make_unique<ComputeShader>("Shaders/agent.comp.glsl");
	m_textureComputeProgram = std::make_unique<ComputeShader>("Shaders/textureProc.comp.glsl");

	m_agentComputeProgram->useShaderStorageBuffer(NUM_AGENTS * sizeof(Agent), (void*)&m_agentSystem.getAgents()[0]);

	m_initialTexture = std::make_unique<Texture>(win_width, win_height, GL_READ_WRITE);
	m_processedTexture = std::make_unique<Texture>(win_width, win_height, GL_WRITE_ONLY);

	m_quad = std::make_unique<Sprite>(-1.0f, 1.0f, 1.0f, -1.0f, m_processedTexture->id);
}

PhysarumSimulation::~PhysarumSimulation()
{
}

void PhysarumSimulation::run()
{
	GLuint groups_a = NUM_AGENTS / 64;
	GLuint groups_x = m_window->getWidth() / 8;
	GLuint groups_y = m_window->getHeight() / 8;

	while (!m_window->windowShouldClose())
	{
		m_window->use();
		float deltaTime = m_window->getDeltaTime();

		m_agentComputeProgram->use();
		m_agentComputeProgram->setFloat("deltaTime", deltaTime);
		m_agentComputeProgram->dispatch(groups_a, 1, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

		m_textureComputeProgram->use();
		m_textureComputeProgram->setFloat("deltaTime", deltaTime);
		m_textureComputeProgram->dispatch(groups_x, groups_y, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_program->use();
		m_quad->draw();

		m_window->unuse();
	}
}
