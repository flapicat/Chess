#include "ChessLayer.h"

#include "renderer/Renderer2D.h"

ChessLayer::ChessLayer()
	:Layer("ChessLayer"), m_CamControl(1600.f/900.f)
{

}

ChessLayer::~ChessLayer()
{

}

void ChessLayer::OnAttach()
{
	m_TileMapTexture = Texture2D::Create("Assets/Textures/Chess.png");
	m_AssetLoader.LoadFromTileMap(m_TileMapTexture);
	m_BoardTexture = Texture2D::Create("Assets/Textures/board.png");
}

void ChessLayer::OnDetach()
{

}

void ChessLayer::Update(TimeStep ts)
{
	m_CamControl.OnUpdate(ts);

	Renderer2D::SetClearColor(glm::vec4(0.1f,0.1f,0.1f,0.1f));
	Renderer2D::ClearBuffer();

	Renderer2D::BeginScene(m_CamControl.GetCamera());
	
	Renderer2D::DrawQuad({ 0.0f,0.0f,0.0f }, { 2.0f,2.0f }, m_BoardTexture);
	for (size_t j = 0; j < 2; j++)
	{
		for (size_t i = 0; i < 6; i++)
		{
			PieceType type;
			if (j == 0) // black
				type = static_cast<PieceType>(i); 
			else // white
				type = static_cast<PieceType>(i + 6); 

			RenederPiece({ i * 0.5f - 1.5f, j - 0.5f, 0.1f }, type);
		}
	}
	Renderer2D::EndScene();
}

void ChessLayer::OnEvent(Event& e)
{
}

void ChessLayer::OnImGuiRender()
{

}

void ChessLayer::input()
{

}

void ChessLayer::RenederPiece(const glm::vec3& pos, PieceType type)
{
	auto& textures = m_AssetLoader.GetPieceTextures();
	Renderer2D::DrawQuad(pos, { 0.2,0.2 }, textures[type]);
}
