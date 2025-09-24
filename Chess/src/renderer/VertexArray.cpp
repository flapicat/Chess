#include "VertexArray.h"
#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:     return GL_FLOAT;
	case ShaderDataType::Float2:    return GL_FLOAT;
	case ShaderDataType::Float3:    return GL_FLOAT;
	case ShaderDataType::Float4:    return GL_FLOAT;
	case ShaderDataType::Mat3:      return GL_FLOAT;
	case ShaderDataType::Mat4:      return GL_FLOAT;
	case ShaderDataType::Int:       return GL_INT;
	case ShaderDataType::Int2:      return GL_INT;
	case ShaderDataType::Int3:      return GL_INT;
	case ShaderDataType::Int4:      return GL_INT;
	case ShaderDataType::Bool:      return GL_BOOL;
	}

	LOG_ERROR("Unknown ShaderDataType!");
	return 0;
}

Ref<VertexArray> VertexArray::Create()
{
	return CreateRef<VertexArray>();
}

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_RendererID);
}
void VertexArray::UnBind()
{
	glBindVertexArray(0);
}
void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();


	uint32_t index = 0;
	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(const void*)element.Offset);
		index++;
	}

	m_vertexBuffers = vertexBuffer;
}
void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();

	m_indexBuffer = indexBuffer;
}