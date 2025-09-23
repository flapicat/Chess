#pragma once
#include <glm/glm.hpp>
#include "Renderer/VertexArray.h"

class RendererImpl
{
public:		
	void Init();
	void SetClearColor(const glm::vec4& clearColor);
	void ClearBuffers();
	void DrawIndexed(Ref<VertexArray>, uint32_t count = 0);

	static Scope<RendererImpl> Create();
};

