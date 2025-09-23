#include "OrthographicCameraController.h"

#include <GLFW/glfw3.h>
#include "Core/Input.h"

	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: m_aspectRatio(aspectRatio),
		m_Camera(-aspectRatio, aspectRatio, -1.0f, 1.0f)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		float deltaTime = ts.GetSeconds();

		/*
		//POSITION
		if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_CameraPosition.x -= m_cameraMovementSpeed * deltaTime;
		}
		if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_CameraPosition.x += m_cameraMovementSpeed * deltaTime;
		}
		if (Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_CameraPosition.y += m_cameraMovementSpeed * deltaTime;
		}
		if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_CameraPosition.y -= m_cameraMovementSpeed * deltaTime;
		}
		if (Input::IsKeyPressed(GLFW_KEY_R))
		{
			m_CameraPosition = { 0.0f,0.0f,0.0f };
		}
		*/
		m_Camera.SetPosition(m_CameraPosition);
	}