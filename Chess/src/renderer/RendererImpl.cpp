#include "RendererImpl.h"
#include <glad/glad.h>

Scope<RendererImpl> RendererImpl::Create()
{
	return CreateScope<RendererImpl>();
}

void RendererImpl::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RendererImpl::SetClearColor(const glm::vec4& clearColor)
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.a);
}

void RendererImpl::ClearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererImpl::DrawIndexed(Ref<VertexArray> VA, uint32_t count)
{
	VA->Bind();
	glDrawElements(GL_TRIANGLES, count > 0 ? count : VA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}