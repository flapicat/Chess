#pragma once
#include<GLFW/glfw3.h>

class TimeStep
{
public:
	TimeStep(float time = 0.0f)
		:m_Time(time)
	{
	}

	operator float() const {
		return m_Time;
	}

	float GetSeconds() const { return m_Time; }
	float GetMilliseconds() const { return m_Time * 1000.0f; }
private:
	float m_Time;
};

class Time
{
public:
	Time() = delete;
	static float GetTime() { return (float)glfwGetTime(); };
};