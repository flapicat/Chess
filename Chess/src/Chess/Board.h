#pragma once
#include <array>
#include <core/core.h>
#include "renderer/Texture.h"
#include "AssetsLoader.h"

static PieceType GetPieceTypeFromString(const std::string& pts);

class Board
{
public:
	Board();
	~Board();

	void CreateBoard();
	void RenderBoard();
	void RenederPiece(const glm::vec3& pos, PieceType type);
private:
	std::array<std::string, 64> m_BoardMap = {};
	Ref<Texture2D> m_Texture;
};

