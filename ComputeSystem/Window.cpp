#include "Window.h"

#include "Shader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "Sprite.h"

const size_t NUM_AGENTS = 131072;

Window::Window(int width, int height, std::string name, int swapInterval, bool isFullscreen) 
	: m_winWidth(width), m_winHeight(height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_win = glfwCreateWindow(width, height, name.c_str(), isFullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (m_win == nullptr)
		std::cout << "Failed to open window.\n";

	glfwMakeContextCurrent(m_win);

	glfwSwapInterval(swapInterval);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD.\n";
	}

	m_agentSystem = AgentSystem(width, height, NUM_AGENTS, PositionMode::CIRCLE);

	m_program = new Shader("Shaders/main.vert.glsl", "Shaders/main.frag.glsl");
	m_agentComputeProgram = new ComputeShader("Shaders/agent.comp.glsl");
	m_textureComputeProgram = new ComputeShader("Shaders/textureProc.comp.glsl");

	m_agentComputeProgram->useShaderStorageBuffer(NUM_AGENTS * sizeof(Agent), (void*)&m_agentSystem.getAgents()[0]);

	m_initialTexture = new Texture(width, height, GL_READ_WRITE);
	m_processedTexture = new Texture(width, height, GL_WRITE_ONLY);

	m_quad = new Sprite(-1.0f, 1.0f, 1.0f, -1.0f, m_processedTexture->id);
}

Window::~Window()
{
	delete m_program, m_agentComputeProgram, m_textureComputeProgram;
	delete m_initialTexture, m_processedTexture, m_quad;
	glfwTerminate();
}

void Window::run()
{
	GLuint groups_a = NUM_AGENTS / 64;
	GLuint groups_x = m_winWidth / 8;
	GLuint groups_y = m_winHeight / 8;

	while (!glfwWindowShouldClose(m_win))
	{
		float currFrame = (float)glfwGetTime();
		m_deltaTime = currFrame - m_prevFrame;
		m_prevFrame = currFrame;

		processInput();

		m_agentComputeProgram->use();
		m_agentComputeProgram->setFloat("deltaTime", m_deltaTime);
		m_agentComputeProgram->dispatch(groups_a, 1, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

		m_textureComputeProgram->use();
		m_agentComputeProgram->setFloat("deltaTime", m_deltaTime);
		m_textureComputeProgram->dispatch(groups_x, groups_y, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_program->use();
		m_quad->draw();

		glfwSwapBuffers(m_win);
		glfwPollEvents();
	}
}

void Window::processInput()
{
	if (glfwGetKey(m_win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_win, true);
	if (glfwGetKey(m_win, GLFW_KEY_F1) == GLFW_PRESS)
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwGetKey(m_win, GLFW_KEY_F2) == GLFW_PRESS)
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	if (glfwGetKey(m_win, GLFW_KEY_F3) == GLFW_PRESS)
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
