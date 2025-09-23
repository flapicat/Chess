#pragma once
#include "OrthographicCamera.h"
#include "core/TimeStep.h"

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio = 0);
		~OrthographicCameraController() {}

		OrthographicCamera& GetCamera() { return m_Camera; }

		void setAspectRatio(float aspectRatio) { 
			m_aspectRatio = aspectRatio;
			m_Camera.updateProjectionMatrix(m_aspectRatio);
		}

		void OnUpdate(TimeStep ts);
	private:
		OrthographicCamera m_Camera;
		float m_aspectRatio = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_cameraMovementSpeed = 1;
	};
