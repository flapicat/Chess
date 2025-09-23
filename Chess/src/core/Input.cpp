#include "Input.h"
#include "Core/App.h"
#include <GLFW/glfw3.h>

std::unordered_map<int, bool> Input::s_KeyStates;

bool Input::IsKeyPressed(int keycode)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
	return glfwGetKey(window, keycode) == GLFW_PRESS;
}

bool Input::IsKeyReleased(int keycode)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
	return glfwGetKey(window, keycode) == GLFW_RELEASE;
}

bool Input::OnKeyPressed(int keycode)
{
    bool pressed = IsKeyPressed(keycode);

    if (pressed && !s_KeyStates[keycode])
    {
        s_KeyStates[keycode] = true;
        return true;
    }

    // update state
    if (!pressed)
        s_KeyStates[keycode] = false;

    return false;
}

bool Input::OnKeyReleased(int keycode)
{
    bool pressed = IsKeyPressed(keycode);

    if (!pressed && s_KeyStates[keycode])
    {
        s_KeyStates[keycode] = false; 
        return true;                  
    }

    if (pressed)
        s_KeyStates[keycode] = true;

    return false;
}
