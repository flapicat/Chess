#pragma once

#include "Buffer.h"

class VertexArray
{
public:
	static Ref<VertexArray> Create();
	VertexArray();
	~VertexArray() {};

	void Bind();
	void UnBind();
	void Delete();

	void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

	const Ref<VertexBuffer>& GetVertexBuffer() const { return m_vertexBuffers; };
	const Ref<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; };
private:
	uint32_t m_RendererID;
	Ref<VertexBuffer> m_vertexBuffers;
	Ref<IndexBuffer> m_indexBuffer;
};
