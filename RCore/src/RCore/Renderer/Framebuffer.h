#pragma once
#include "rcpch.h"

#include "Renderer.h"

class Framebuffer
{
public:
	Framebuffer(int width, int height);
	~Framebuffer();

	void Bind();
	void Unbind();

	void Resize(uint32_t width, uint32_t height);

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }

	uint32_t GetColorAttachmentRendererID() const { return m_Framebuffer.texture.id; };
	RenderTexture GetTarget() const { return m_Framebuffer; }

private:
	uint32_t m_Width, m_Height;
	RenderTexture m_Framebuffer;
};
