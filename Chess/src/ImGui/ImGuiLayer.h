#pragma once
#include "Core/Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Event& e) override;
	virtual void OnImGuiRender() override;

	void Begin();
	void End();

private:
	float m_Time = 0.0f;
};