#include "Board.h"
#include "renderer/Renderer2D.h"

static Ref<Piece> GetPieceTypeFromString(const std::string& pts)
{
	if (pts == "  ") return CreateRef<Piece>(PieceType::null);
	if (pts == "bR") return CreateRef<Piece>(PieceType::bRook);
	if (pts == "bS") return CreateRef<Piece>(PieceType::bKnight);
	if (pts == "bB") return CreateRef<Piece>(PieceType::bBishop);
	if (pts == "bQ") return CreateRef<Piece>(PieceType::bQueen);
	if (pts == "bK") return CreateRef<Piece>(PieceType::bKing);
	if (pts == "bP") return CreateRef<Piece>(PieceType::bPawn);

	if (pts == "wR") return CreateRef<Piece>(PieceType::wRook);
	if (pts == "wS") return CreateRef<Piece>(PieceType::wKnight);
	if (pts == "wB") return CreateRef<Piece>(PieceType::wBishop);
	if (pts == "wQ") return CreateRef<Piece>(PieceType::wQueen);
	if (pts == "wK") return CreateRef<Piece>(PieceType::wKing);
	if (pts == "wP") return CreateRef<Piece>(PieceType::wPawn);
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

	for (auto& piece : m_BoardMap)
	{
		m_Pieces.push_back(GetPieceTypeFromString(piece));
	}
}

void Board::RenderBoard()
{
	int x = 0;
	int y = 0;
	for (auto& piece : m_Pieces)
	{
		piece->SetPos({x*0.2-0.5,y*0.2 - 0.5,0.1});
		RenderPiece(piece);
		x++;
		if (x == 8)
		{
			x = 0;
			y++;
		}
	}
}

void Board::RenderPiece(const Ref<Piece>& piece)
{
	if (piece->getTexture() != nullptr)
	{
		Renderer2D::DrawQuad(piece->GetPos(), { 0.2,0.2 }, piece->getTexture());
	}
}