#pragma once
#include <unordered_map>
#include "core/core.h"
#include "renderer/Texture.h"
#include "renderer/SubTexture.h"

#include "Piece.h"

class AssetsLoader
{
public:
	AssetsLoader();
	~AssetsLoader();

	void LoadFromTileMap(Ref<Texture2D> m_Tilemap);
	static std::unordered_map<PieceType, Ref<SubTexture2D>>& GetPieceTextures() { return m_PieceTextures; }
private:
	static std::unordered_map<PieceType, Ref<SubTexture2D>> m_PieceTextures;
};

