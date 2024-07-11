#include "Window.h"
typedef int* int_ptr;
GLFWwindow* Window::window;

int16_t Window::width = 0;
int16_t Window::height = 0;

int Window::instalization(const int16_t width, const int16_t height, const char* name)
{
	Window::width = width;
	Window::height = height;
	//Init GLFW
	glfwInit();
	// Init GLFW
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	int FrameBufferWidth = 10, FrameBufferHeight = 10;
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &FrameBufferWidth, &FrameBufferHeight);
	glViewport(0, 0, FrameBufferWidth, FrameBufferHeight);
	//glViewport(0, 0, FrameBufferWidth, FrameBufferHeight);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
}

int16_t Window::getWidth()
{
	return Window::width;
}

int16_t Window::getHeight()
{
	return Window::height;
}


void Window::setCursorMode(int mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::shouldClose(bool flag)
{
	glfwSetWindowShouldClose(window, flag);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::Terminate()
{
	glfwTerminate();
}

GLFWwindow* Window::getWindow()
{
	return window;
}
