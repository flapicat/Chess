#pragma once
#include "RendererImpl.h"
#include "Texture.h"
#include "SubTexture.h"
#include "OrthographicCamera.h"

class Renderer2D
{
public:
	struct Statistic
	{
		uint32_t DrawCalls = 0;
		uint32_t NumOfQuads = 0;

		void Reset() { DrawCalls = 0; NumOfQuads = 0; }

		uint32_t GetQuadVertices() const { return NumOfQuads * 4; }
		uint32_t GetQuadIndices() const { return NumOfQuads * 6; }
	};
inline static Statistic s_Statistic;

public:
	static void Init();

	static void SetClearColor(const glm::vec4& clearColor);
	static void ClearBuffer();
	static void DrawIndexed(Ref<VertexArray> VA, uint32_t count);

	static void BeginScene(const OrthographicCamera& camera);
	static void EndScene();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void DrawQuadInternal(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);
	static void DrawQuadInternalTransform(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);
	static void DrawRotatedQuadInternal(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);
	
	static void DrawQuadInternalSubTex(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> texture, float tilingFactor = 1);
	static void DrawRotatedQuadInternalSubTex(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> texture, float tilingFactor = 1);

	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor = 1);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<SubTexture2D> texture, float tilingFactor = 1);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> texture, float tilingFactor = 1);

	static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
	static void DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color);
	
	static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
	static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor = 1);
	static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);
	static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<SubTexture2D> texture, float tilingFactor = 1);
	static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<SubTexture2D> texture, float tilingFactor = 1);

	static Statistic& GetStatistic() { return s_Statistic; };
private:
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};
	inline static SceneData s_SceneData;

	static Scope<RendererImpl> s_RendererImpl;
};