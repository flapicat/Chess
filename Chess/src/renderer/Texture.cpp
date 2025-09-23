#include "Texture.h"
#include "Core/Log.h"

#include <stb_image.h>

Ref<Texture2D> Texture2D::Create()
{
	return CreateRef<Texture2D>();
}
Ref<Texture2D> Texture2D::Create(const std::string& path)
{
	return CreateRef<Texture2D>(path);
}

Ref<Texture2D> Texture2D::Create(int width, int height, unsigned char* data)
{
	 return CreateRef<Texture2D>(width, height, data);
}

Texture2D::Texture2D()
{
	glGenTextures(1, &m_RendererID);
}

Texture2D::Texture2D(const std::string& path)
{
	glGenTextures(1, &m_RendererID);
	load(path);
}

Texture2D::Texture2D(int width, int height, unsigned char* data)
	:m_width(width), m_height(height), m_format(GL_RGBA)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture2D::load(const std::string& path)
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	int nrChannels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &nrChannels, 0);
	if (!data)
	{
		LOG_WARN("Faild to load a texture from the path: {0}", path);
	}

	if (nrChannels == 3)
	{
		m_format = GL_RGB;
	}
	if (nrChannels == 4)
	{
		m_format = GL_RGBA;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture2D::setData(unsigned char* data)
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);

}

void Texture2D::Bind(uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::UnBind(uint32_t slot)
{
	glBindTextureUnit(slot, 0);
}