#pragma once
#include <array>
#include <core/core.h>
#include "renderer/Texture.h"
#include "AssetsLoader.h"
#include "Piece.h"

static Ref<Piece> GetPieceTypeFromString(const std::string& pts);

class Board
{
public:
	Board();
	~Board();

	void CreateBoard();
	void RenderBoard();
	void RenderPiece(const Ref<Piece>& piece);
private:
	std::array<std::string, 64> m_BoardMap = {};
	std::vector<Ref<Piece>> m_Pieces = {};
	Ref<Texture2D> m_Texture;
};

