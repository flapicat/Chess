#include "Window.h"
#include "Log.h"


#include <Event/AppEvent.h>
#include <Event/MouseEvent.h>
#include <Event/KeyboardEennt.h>


Scope<Window> Window::CreateNewWindow(const WindowProp& props)
{
	return CreateScope<Window>(props);
}

Window::Window(const WindowProp& props)
{
	Create(props);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void Window::SetEventCallback(const std::function<void(Event&)>& callback)
{
	m_winData.EvCallback = callback;
}

GLFWwindow* Window::GetNativeWindow() const
{
	return m_window;
}

void Window::Create(const WindowProp& props)
{
	m_winData.Width = props.Width;
	m_winData.Height = props.Height;
	m_winData.Title = props.Title;

	if (!glfwInit())
	{
		LOG_ERROR("Failed to init GLFW!");
	}

	m_window = glfwCreateWindow(m_winData.Width, m_winData.Height, m_winData.Title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		LOG_ERROR("Failed to initialize GLFW window!");
	}

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	if (primary)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(primary);

		if (mode)
		{
			int windowWidth, windowHeight;
			glfwGetWindowSize(m_window, &windowWidth, &windowHeight);

			int xpos = (mode->width - windowWidth) / 2;
			int ypos = (mode->height - windowHeight) / 2;

			glfwSetWindowPos(m_window, xpos, ypos);
		}
	}

	glfwSetWindowUserPointer(m_window, this);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(true);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
	{
		LOG_ERROR("Failed to initialize Glad!");
	}

	LOG_INFO("OpenGL info:");
	LOG_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
	LOG_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
	LOG_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {

		glViewport(0, 0, width, height);
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_winData.Width = width;
		win->m_winData.Height = height;

		WindowResizeEvent event(width, height);
		win->m_winData.EvCallback(event);
		});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		win->m_winData.EvCallback(event);
		});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		MouseScrollEvent event((int)xOffset, (int)yOffset);
		win->m_winData.EvCallback(event);
		});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			KeyPressedEvent event(key);
			win->m_winData.EvCallback(event);
		});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {

		});
}