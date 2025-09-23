#pragma once
#include <array>
#include <glm/glm.hpp>

#include "Texture.h"

class SubTexture2D
{
public:
	SubTexture2D(const Ref<Texture2D>& tilemapTexture, const glm::vec2& min, const glm::vec2& max);

	static Ref<SubTexture2D> CreateTextureFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);

	const Ref<Texture2D> GetTexture() const { return m_Texture; };
	const std::array<glm::vec2, 4>& GetTextureCoords() const { return texCoords; }
private:
	Ref<Texture2D> m_Texture;
	std::array<glm::vec2, 4> texCoords;
};