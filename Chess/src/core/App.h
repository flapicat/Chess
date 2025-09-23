#pragma once
#include "Event/AppEvent.h"
#include "core.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

class App
{
public:
	App();
	~App();

	void Run();
	void Close();
	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	void OnEvent(Event& event);

	inline Window& GetWindow() const { return *m_Window; }

	inline static App& Get() { return *s_Instance; }
private:
	bool OnWindowCloseEvent(const WindowCloseEvent& e);
private:
	bool m_Running = true;
	static App* s_Instance;
	Scope<Window>m_Window;
	LayerStack m_LayerStack;
	ImGuiLayer* m_ImGuiLayer;
	float m_LastFrame;
};

