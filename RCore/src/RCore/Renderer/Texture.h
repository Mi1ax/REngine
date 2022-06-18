#pragma once

#include "rcpch.h"
#include <rlgl.h>

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetID() const { return m_RendererID; }

	void Bind() const;
	void Unbind() const;

private:
	std::string m_Path;
	uint32_t m_Width, m_Height;
	uint32_t m_RendererID;
	rlPixelFormat m_DataFormat;
};
