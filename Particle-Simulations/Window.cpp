#include "Window.hpp"

Window::Window(int width, int height, const std::string& name, int swapInterval, bool isFullscreen) 
	: m_winWidth(width), m_winHeight(height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_win = glfwCreateWindow(width, height, name.c_str(), isFullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (m_win == nullptr)
	{
		std::cout << "Failed to open window.\n";
	}

	glfwMakeContextCurrent(m_win);

	glfwSwapInterval(swapInterval);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD.\n";
	}
}

Window::~Window()
{
	glfwTerminate();
}

void Window::use()
{
	float currFrame{ static_cast<float>(glfwGetTime()) };
	m_deltaTime = currFrame - m_prevFrame;
	m_prevFrame = currFrame;

	processInput();
}

void Window::unuse()
{
	glfwSwapBuffers(m_win);
	glfwPollEvents();
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
