#include "Piece.h"
#include "AssetsLoader.h"

Piece::Piece(PieceType type)
	:m_Type(type)
{
	m_Pos = { 0.0f,0.0f,0.1f };
	m_Texture = nullptr;

	m_Texture = AssetsLoader::GetPieceTextures()[m_Type];
}

Piece::~Piece()
{
}
