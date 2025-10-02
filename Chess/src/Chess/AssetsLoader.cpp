#include "AssetsLoader.h"

constexpr glm::vec2 SpriteSize = { 64,64 };

std::unordered_map<PieceType, Ref<SubTexture2D>> AssetsLoader::m_PieceTextures;

AssetsLoader::AssetsLoader()
{
}

AssetsLoader::~AssetsLoader()
{
}

void AssetsLoader::LoadFromTileMap(Ref<Texture2D> m_Tilemap)
{
    for (size_t j = 0; j < 2; j++) 
    {
        for (int i = 0; i < 6; i++) 
        {
            Ref<SubTexture2D> m_subTex = SubTexture2D::CreateTextureFromCoords(m_Tilemap, { i, j }, SpriteSize);

            PieceType type;
            if (j == 0) // white pieces
                type = static_cast<PieceType>(i+1);
            else        // black pieces
                type = static_cast<PieceType>(i + 7);

            m_PieceTextures[type] = m_subTex;
        }
    }
}
