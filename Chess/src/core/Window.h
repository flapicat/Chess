#pragma once
#include "core.h"
#include <functional>
#include <string>
#include "Event/Event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowProp
{
	int Width;
	int Height;
	std::string Title;
	WindowProp(int width = 1600, int height = 900, const std::string& title = "Nexus Engine")
		:Width(width), Height(height), Title(title)
	{
	}
};

class Window
{
public:
	static Scope<Window> CreateNewWindow(const WindowProp& props);
	Window(const WindowProp& props);
	~Window();

	void OnUpdate();

	unsigned int GetWidth() const { return m_winData.Width; }
	unsigned int GetHeight() const { return m_winData.Height; }

	void SetEventCallback(const std::function<void(Event&)>& callback);


	void SetVSync(bool enabled) { glfwSwapInterval(enabled); };
	bool IsVSync() { return m_winData.VSync; };

	GLFWwindow* GetNativeWindow() const;

	void Create(const WindowProp& props = WindowProp());
private:
	GLFWwindow* m_window;

	struct WindowData
	{
		int Width;
		int Height;
		std::string Title;

		bool VSync = true;

		std::function<void(Event&)> EvCallback;
	};

	WindowData m_winData;
};