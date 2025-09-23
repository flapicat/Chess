#pragma once
#include "Event.h"

class MouseScrollEvent : public Event
{
public:
	MouseScrollEvent(int xOffset, int yOffset)
		:m_xOffset(xOffset), m_yOffset(yOffset)
	{
		SetCategory(EventCategory::Mouse);
	}

	int GetXoffset() const { return m_xOffset; }
	int GetYoffset() const { return m_yOffset; }
private:
	int m_xOffset, m_yOffset;
};

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(int xPos, int yPos)
		:m_xpos(xPos), m_ypos(yPos)
	{
		SetCategory(EventCategory::Mouse);
	}

	int GetXpos() const { return m_xpos; }
	int GetYpos() const { return m_ypos; }
private:
	int m_xpos, m_ypos;
};