#pragma once
#include <string>

#include "Core/TimeStep.h"
#include "Event/Event.h"

class Layer
{
public:
	Layer(const std::string& name)
		:m_DebugName(name)
	{
	}

	virtual void OnAttach() {};
	virtual void OnDetach() {};
	virtual void Update(TimeStep ts) {}
	virtual void OnEvent(Event& e) {};
	virtual void OnImGuiRender() {};
private:
	std::string m_DebugName;
};