#ifndef WINDOW_H
#define WINDOW_H
#include "lib.h"
class GLFWWwindow;
class Window
{
private:
	static GLFWwindow* window;
	static int16_t width;
	static int16_t height;
protected:
public:
	static int instalization(const int16_t width = 800, const int16_t height = 600, const char* name = "TEST");

	static int16_t getWidth();
	static int16_t getHeight();
	static void setCursorMode(int mode);
	static bool shouldClose();
	static void shouldClose(bool flag);
	static void swapBuffers();
	static void Terminate();	
	static GLFWwindow* getWindow();
};

#endif // !WINDOW_H


