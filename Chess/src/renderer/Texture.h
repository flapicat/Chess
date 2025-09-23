#pragma once
#include "Core/core.h"
#include <string>
#include <glad/glad.h>

class Texture2D 
{
public:
	static Ref<Texture2D> Texture2D::Create();
	static Ref<Texture2D> Texture2D::Create(const std::string& path);
	static Ref<Texture2D> Texture2D::Create(int width, int height, unsigned char* data);

	Texture2D();
	Texture2D(const std::string& path);
	Texture2D(int width, int height, unsigned char* data);
	~Texture2D();


	void load(const std::string& path) ;
	uint32_t GetWidth() const  { return m_width; };
	uint32_t GetHeight() const  { return m_height; };
	void setData(unsigned char* data) ;
	void Bind(uint32_t slot = 0) ;
	void UnBind(uint32_t slot = 0) ;
	uint32_t GetRendererID() { return m_RendererID; };
private:
	uint32_t m_RendererID;
	int m_width, m_height;
	GLenum m_format;
};