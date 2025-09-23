#include "Renderer2D.h"

#include "Renderer/Shader.h"

#include <glm/glm.hpp>          
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

Scope<RendererImpl> Renderer2D::s_RendererImpl = RendererImpl::Create();

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TextureCoords;
	float TextureID;
	float TailingFactor;
};

struct Rendererdata
{
	//Const
	const uint32_t MaxQuads = 10000;
	const uint32_t MaxVertices = MaxQuads * 4;
	const uint32_t MaxIndices = MaxQuads * 6;
	const static uint32_t MaxTextureSlots = 32;

	//Shaders
	Ref<Shader> TextureShader;

	Ref<VertexArray> VertexArray;
	Ref<VertexBuffer> VertexBuffer;

	//QuadData
	QuadVertex* QuadVertexBufferBase;
	QuadVertex* QuadVertexBufferPtr;

	uint32_t QuadIndexCount = 0;

	//Textures
	Ref<Texture2D> WhiteTexture;
	uint32_t WhiteTextureID = 0;
	Ref<Texture2D> TextureSlots[MaxTextureSlots];
	uint32_t TextureSlotIndex = 1; // 0 = white texture
};

static Rendererdata s_Data;

void Renderer2D::Init()
{
	s_RendererImpl->Init();

	s_Data.TextureShader = Shader::Create("Assets/Shaders/TextureShader.glsl");

	s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

	s_Data.VertexArray = VertexArray::Create();
	s_Data.VertexArray->Bind();

	s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color"},
		{ ShaderDataType::Float2, "a_TextureCoords"},
		{ ShaderDataType::Float , "a_TextureID"},
		{ ShaderDataType::Float , "a_TailingFactor"}
	};
	s_Data.VertexBuffer->SetLayout(layout);
	s_Data.VertexArray->SetVertexBuffer(s_Data.VertexBuffer);

	uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
	s_Data.VertexArray->SetIndexBuffer(quadIB);
	delete[] quadIndices;

	unsigned char WhiteTextureData[4] = {255,255,255,255};
	s_Data.WhiteTexture = Texture2D::Create(1,1, WhiteTextureData);
	s_Data.WhiteTextureID = 0;
	s_Data.TextureSlots[0] = s_Data.WhiteTexture;

	s_Data.VertexArray->UnBind();

	int samplers[s_Data.MaxTextureSlots];
	for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		samplers[i] = i;

	s_Data.TextureShader->Bind();
	s_Data.TextureShader->SetIntArray("u_Textures", s_Data.MaxTextureSlots, samplers);
}

void Renderer2D::SetClearColor(const glm::vec4& clearColor)
{
	s_RendererImpl->SetClearColor(clearColor);
}

void Renderer2D::ClearBuffer()
{
	s_RendererImpl->ClearBuffers();
}

void Renderer2D::DrawIndexed(Ref<VertexArray> VA, uint32_t count)
{
	s_RendererImpl->DrawIndexed(VA, count);
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
	s_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	s_Data.TextureShader->Bind();
	s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", s_SceneData.ViewProjectionMatrix);

	BeginBatch();
}

void Renderer2D::EndScene()
{
	EndBatch();
}

void Renderer2D::BeginBatch()
{
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	s_Data.QuadIndexCount = 0;

	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::EndBatch()
{
	uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
	if (dataSize > 0)
		s_Data.VertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

	Flush();
}

//Quad
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	DrawQuadInternal(position, size, color, s_Data.WhiteTexture);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor)
{
	DrawQuadInternal(position, size, glm::vec4(1.0f), texture, tilingFactor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
{
	DrawQuadInternal(position, size, color, texture, tilingFactor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<SubTexture2D> texture, float tilingFactor)
{
	DrawQuadInternalSubTex(position, size, glm::vec4(1.0f), texture, tilingFactor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> texture, float tilingFactor)
{
	DrawQuadInternalSubTex(position, size, color, texture, tilingFactor);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	DrawQuadInternalTransform(transform, color, s_Data.WhiteTexture);
}

//Rotated Quad
void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color)
{
	DrawQuadInternalTransform(transform, color, s_Data.WhiteTexture);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
{
	DrawRotatedQuadInternal(position, rotation, size, color, s_Data.WhiteTexture);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor)
{
	DrawRotatedQuadInternal(position, rotation, size, glm::vec4(1.0f), texture, tilingFactor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
{
	DrawRotatedQuadInternal(position, rotation, size, color, texture, tilingFactor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<SubTexture2D> texture, float tilingFactor)
{
	DrawRotatedQuadInternalSubTex(position, rotation, size, glm::vec4(1.0f), texture, tilingFactor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> texture, float tilingFactor)
{
	DrawRotatedQuadInternalSubTex(position, rotation, size, color, texture, tilingFactor);
}

//Internals
void Renderer2D::DrawQuadInternal(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
	{
		EndBatch();
		BeginBatch();
	}

	float textureIndex = 0.0f;
	if (texture != nullptr && texture != s_Data.WhiteTexture)
	{
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
		{
			if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) // new texture
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size,1.0f));

	glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
	glm::vec3 positions[4] = {
		{ -0.5f, -0.5f, 0.0f }, // bottom left
		{  0.5f, -0.5f, 0.0f }, // bottom right
		{  0.5f,  0.5f, 0.0f }, // top right
		{ -0.5f,  0.5f, 0.0f }  // top left
	};

	for (int i = 0; i < 4; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
		s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
		s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;

	s_Statistic.NumOfQuads++;
}

void Renderer2D::DrawQuadInternalTransform(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
	{
		EndBatch();
		BeginBatch();
	}

	float textureIndex = 0.0f;
	if (texture != nullptr && texture != s_Data.WhiteTexture)
	{
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
		{
			if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) // new texture
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
	}

	glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
	glm::vec3 positions[4] = {
		{ -0.5f, -0.5f, 0.0f }, // bottom left
		{  0.5f, -0.5f, 0.0f }, // bottom right
		{  0.5f,  0.5f, 0.0f }, // top right
		{ -0.5f,  0.5f, 0.0f }  // top left
	};

	for (int i = 0; i < 4; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
		s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
		s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;

	s_Statistic.NumOfQuads++;
}

void Renderer2D::DrawRotatedQuadInternal(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
	{
		EndBatch();
		BeginBatch();
	}

	float textureIndex = 0.0f;
	if (texture != nullptr && texture != s_Data.WhiteTexture)
	{
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
		{
			if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) // new texture
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0,0,1)) * 
		glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

	glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
	glm::vec3 positions[4] = {
		{ -0.5f, -0.5f, 0.0f }, // bottom left
		{  0.5f, -0.5f, 0.0f }, // bottom right
		{  0.5f,  0.5f, 0.0f }, // top right
		{ -0.5f,  0.5f, 0.0f }  // top left
	};

	for (int i = 0; i < 4; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
		s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
		s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;

	s_Statistic.NumOfQuads++;
}

void Renderer2D::DrawQuadInternalSubTex(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> subTexture, float tilingFactor)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
	{
		EndBatch();
		BeginBatch();
	}

	float textureIndex = 0.0f;
	if (subTexture != nullptr && subTexture->GetTexture() != s_Data.WhiteTexture)
	{
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
		{
			if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == subTexture->GetTexture()->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) // new texture
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = subTexture->GetTexture();
			s_Data.TextureSlotIndex++;
		}
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

	auto& texCoords = subTexture->GetTextureCoords();
	//glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
	glm::vec3 positions[4] = {
		{ -0.5f, -0.5f, 0.0f }, // bottom left
		{  0.5f, -0.5f, 0.0f }, // bottom right
		{  0.5f,  0.5f, 0.0f }, // top right
		{ -0.5f,  0.5f, 0.0f }  // top left
	};

	for (int i = 0; i < 4; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
		s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
		s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;

	s_Statistic.NumOfQuads++;
}

void Renderer2D::DrawRotatedQuadInternalSubTex(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> subTexture, float tilingFactor)
{
	if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
	{
		EndBatch();
		BeginBatch();
	}

	float textureIndex = 0.0f;
	if (subTexture != nullptr && subTexture->GetTexture() != s_Data.WhiteTexture)
	{
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
		{
			if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == subTexture->GetTexture()->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) // new texture
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = subTexture->GetTexture();
			s_Data.TextureSlotIndex++;
		}
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

	auto& texCoords = subTexture->GetTextureCoords();
	//glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
	glm::vec3 positions[4] = {
		{ -0.5f, -0.5f, 0.0f }, // bottom left
		{  0.5f, -0.5f, 0.0f }, // bottom right
		{  0.5f,  0.5f, 0.0f }, // top right
		{ -0.5f,  0.5f, 0.0f }  // top left
	};

	for (int i = 0; i < 4; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
		s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
		s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;

	s_Statistic.NumOfQuads++;
}

void Renderer2D::Flush()
{
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		s_Data.TextureSlots[i]->Bind(i);

	s_RendererImpl->DrawIndexed(s_Data.VertexArray, s_Data.QuadIndexCount);
	s_Statistic.DrawCalls++;
}

