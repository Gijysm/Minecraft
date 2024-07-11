#include "Event.h"
bool* Event::_keys;
uint*  Event::_frames;
uint Event::_current = 0;
float Event::deltaX = 0.0f;
float Event::deltaY = 0.0f;
float Event::x = 0.0f;
float Event::y = 0.0f;
bool Event::_cursor_locked = false;
bool Event::_cursor_started = false;
void GLFW_resize_Window(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
float& Event::getDeltaX()
{
    return deltaX;
}
float& Event::getDeltaY()
{
    return deltaY;
}
bool& Event::getCursorLocked()
{
    return _cursor_locked;
}
void Event::toogleCursor()
{
   _cursor_locked = !_cursor_locked;
   Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
int Event::instalization()
{
    GLFWwindow* window = Window::getWindow();
    _keys = new bool[1032];
    _frames = new uint[1032];

    memset(_keys, false, 1032* sizeof(bool));
    memset(_frames, 0, 1032 * sizeof(bool));

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, GLFW_resize_Window);
    return 0;
}

void Event::PullEvents()
{
    _current++;
    deltaX = 0.0f;
    deltaY = 0.0f;
    glfwPollEvents();
}

void Event::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        Event::_keys[key] = true;
        Event::_frames[key] = Event::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Event::_keys[key] = false;
        Event::_frames[key] = Event::_current;
    }
}

void Event::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Event::_cursor_started)
    {
        Event::deltaX = xpos - Event::x;
        Event::deltaY = ypos - Event::y;
    }
    else
    {
        Event::_cursor_started = true;
    }
    Event::x = xpos;
    Event::y = ypos;
}

void Event::MouseCallback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Event::_keys[_MOUSE_BUTTON + button] = true;
        Event::_frames[_MOUSE_BUTTON + button] = Event::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Event::_keys[_MOUSE_BUTTON + button] = false;
        Event::_frames[_MOUSE_BUTTON + button] = Event::_current;
    }
}

bool Event::isKeyPressed(int key)
{
    if(key < 0 || key >= _MOUSE_BUTTON)
        return false;
    return _keys[key];
    cout<< _keys[key] << "\n";
}

bool Event::justReleased(int key)
{
    if (key < 0 || key >= _MOUSE_BUTTON)
        return false;
    return _keys[key] && _frames[key] == _current;
}

bool Event::Clicked(int key)
{
    int index = _MOUSE_BUTTON + key;
    return _keys[index];
}

bool Event::justClicked(int key)
{
    int index = _MOUSE_BUTTON + key;
    return _keys[index] && _frames[index] == _current;
}
