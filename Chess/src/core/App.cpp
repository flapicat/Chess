#include "App.h"

#include "Renderer/Renderer2D.h"

App* App::s_Instance = nullptr;

App::App()
{
	s_Instance = this;
	WindowProp props(1600, 900, "Nexus Engine");
	m_Window = Window::CreateNewWindow(props);
	m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

	m_ImGuiLayer = new ImGuiLayer;
	PushLayer(m_ImGuiLayer);
	Renderer2D::Init();
}

App::~App()
{
}

void App::Run()
{
	while (m_Running)
	{
		float time = Time::GetTime();
		TimeStep timeStep = time - m_LastFrame;
		m_LastFrame = time;

		for (auto& layer : m_LayerStack)
			layer->Update(timeStep);

		m_ImGuiLayer->Begin();
		for (auto& layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnUpdate();
	}
}

void App::Close()
{
	m_Running = false;
}

void App::OnEvent(Event& event)
{
	EventHandler hander(event);
	hander.Handle<WindowCloseEvent>([this](const WindowCloseEvent& ev) {OnWindowCloseEvent(ev); });

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		(*it)->OnEvent(event);
}

void App::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}
void App::PopLayer(Layer* layer)
{
	layer->OnDetach();
	m_LayerStack.PopLayer(layer);
}

bool App::OnWindowCloseEvent(const WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}