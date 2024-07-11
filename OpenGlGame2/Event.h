#ifndef EVENT_H
#define EVENT_H
#include "Window.h"
#include "Defines.h"

class Event
{
private:
	static bool* _keys;
	static uint* _frames;
	static uint _current;
	static float deltaX;
	static float deltaY;
	static float x;
	static float y;
	static bool _cursor_locked;
	static bool _cursor_started;
public:
	static float& getDeltaX();
	static float& getDeltaY();
	static bool& getCursorLocked();
	static void toogleCursor();
	static int instalization();
	static void PullEvents();
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void MouseCallback(GLFWwindow* window, int button, int action, int mode);
	static bool isKeyPressed(int key);
	static bool justReleased(int key);
	static bool Clicked(int key);
	static bool justClicked(int key);
};

#endif