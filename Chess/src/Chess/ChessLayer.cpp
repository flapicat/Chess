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
	m_Board.CreateBoard();
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
	m_Board.RenderBoard();

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