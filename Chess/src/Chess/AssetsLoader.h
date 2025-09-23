#pragma once
#include <unordered_map>
#include "core/core.h"
#include "renderer/Texture.h"
#include "renderer/SubTexture.h"

enum PieceType
{
	bKnight, bBishop, bRook, bQueen, bKing, bPawn,
	wKnight, wBishop, wRook, wQueen, wKing, wPawn
};

class AssetsLoader
{
public:
	AssetsLoader();
	~AssetsLoader();

	void LoadFromTileMap(Ref<Texture2D> m_Tilemap);
	std::unordered_map<PieceType, Ref<SubTexture2D>>& GetPieceTextures() { return m_PieceTextures; }
private:
	std::unordered_map<PieceType, Ref<SubTexture2D>> m_PieceTextures;
};

