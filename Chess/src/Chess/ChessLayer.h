#pragma once
#include "core/Layer.h"
#include "Event/Event.h"
#include "core/core.h"

#include "renderer/Texture.h"
#include "renderer/SubTexture.h"
#include "renderer/OrthographicCameraController.h"

#include "AssetsLoader.h"

class ChessLayer : public Layer
{
public:
	ChessLayer();
	~ChessLayer();

	void OnAttach();
	void OnDetach();
	void Update(TimeStep ts);
	void OnEvent(Event& e);
	void OnImGuiRender();
	void input();

	void RenederPiece(const glm::vec3& pos, PieceType type);
private:
	AssetsLoader m_AssetLoader;
	OrthographicCameraController m_CamControl;
	Ref<Texture2D> m_BoardTexture;
	Ref<Texture2D> m_TileMapTexture;
	Ref<SubTexture2D> m_King;
};

