#include "Board.h"
#include "renderer/Renderer2D.h"

static PieceType GetPieceTypeFromString(const std::string& pts)
{
	if (pts == "bR") return PieceType::bRook;
	if (pts == "bS") return PieceType::bKnight;
	if (pts == "bB") return PieceType::bBishop;
	if (pts == "bQ") return PieceType::bQueen;
	if (pts == "bK") return PieceType::bKing;
	if (pts == "bP") return PieceType::bPawn;

	if (pts == "wR") return PieceType::wRook;
	if (pts == "wS") return PieceType::wKnight;
	if (pts == "wB") return PieceType::wBishop;
	if (pts == "wQ") return PieceType::wQueen;
	if (pts == "wK") return PieceType::wKing;
	if (pts == "wP") return PieceType::wPawn;
}

Board::Board()
{
}

Board::~Board()
{
}

void Board::CreateBoard()
{
	m_BoardMap = {
		"bR", "bS", "bB", "bQ", "bK", "bB", "bS", "bR",
		"bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP",
		"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
		"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
		"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
		"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",
		"wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP",
		"wR", "wS", "wB", "wQ", "wK", "wB", "wS", "wR",
	};
}

void Board::RenderBoard()
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < m_BoardMap.size(); i++)
	{
		if (m_BoardMap[i] != "  ")
		{
			RenederPiece({ x * 0.2 - 1, y * 0.2 - 0.5, 0.1f }, GetPieceTypeFromString(m_BoardMap[i]));
		}
		x++;
		if (x == 8)
		{
			x = 0;
			y++;
		}
	}
}


void Board::RenederPiece(const glm::vec3& pos, PieceType type)
{
	auto& textures = AssetsLoader::GetPieceTextures();
	Renderer2D::DrawQuad(pos, { 0.2,0.2 }, textures[type]);
}
