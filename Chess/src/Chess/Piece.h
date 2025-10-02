#pragma once
#include "core/core.h"
#include "renderer/SubTexture.h"
#include <glm/glm.hpp>

enum PieceType
{
	null,
	bRook, bKnight, bBishop, bQueen, bKing, bPawn,
	wRook, wKnight, wBishop, wQueen, wKing, wPawn
};

class Piece
{
public:
	Piece(PieceType type);
	~Piece();

	PieceType GetType() const { return m_Type; }
	void SetType(PieceType type) { m_Type = type; }

	const glm::vec3& GetPos() const { return m_Pos; }
	void SetPos(const glm::vec3& pos) { m_Pos = pos; }

	const Ref<SubTexture2D>& getTexture() const { return m_Texture; }
private:
	PieceType m_Type;
	glm::vec3 m_Pos;
	Ref<SubTexture2D> m_Texture;
};

