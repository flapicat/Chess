#include "Buffer.h"

#include <glad/glad.h>

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    return CreateRef<VertexBuffer>(size);
}

VertexBuffer::VertexBuffer(uint32_t size)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::SetData(const void* data, uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

////////////////////////////////////////////////////////////////////////////////

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
    return CreateRef<IndexBuffer>(indices, count);
}

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Delete()
{
    glDeleteBuffers(1, &m_RendererID);
}
