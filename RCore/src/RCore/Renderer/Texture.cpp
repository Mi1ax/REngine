#include "rcpch.h"
#include "Texture.h"

#include <stb_image.h>
#include <rlgl.h>

Texture::Texture(const std::string& filePath)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	CORE_ASSERT(data, "Failed to load image!");
	m_Width = width;
	m_Height = height;

	rlPixelFormat dataFormat;
	if (channels == 4)
		dataFormat = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	else if (channels == 3)
		dataFormat = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8;
	
	m_DataFormat = dataFormat;
	CORE_ASSERT(dataFormat, "Format not supported!");

	m_RendererID = rlLoadTexture(data, m_Width, m_Height, m_DataFormat, 1);

	stbi_image_free(data);
}

Texture::~Texture()
{
	rlUnloadTexture(m_RendererID);
}

void Texture::Bind() const
{
	rlEnableTexture(m_RendererID);
}

void Texture::Unbind() const
{
	rlDisableTexture();
}