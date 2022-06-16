#include "rcpch.h"
#include "Framebuffer.h"

#include <iostream>

static const uint32_t s_MaxFramebufferSize = 8192;

Framebuffer::Framebuffer(int width, int height)
	: m_Width(width), m_Height(height)
{
    m_Framebuffer = Renderer::LoadRenderTexture(m_Width, m_Height);
}

Framebuffer::~Framebuffer()
{
    Renderer::UnloadTexture(m_Framebuffer.texture.id);
    Renderer::UnloadRenderTexture(m_Framebuffer);
}

void Framebuffer::Bind()
{
    Renderer::BindFramebuffer(m_Framebuffer.id);
}

void Framebuffer::Unbind()
{
    Renderer::UnbindFramebuffer();
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
        CORE_ERROR("Attempted to resize framebuffer to {0}, {1}", width, height);
        return;
    }

    m_Width = width;
    m_Height = height;
    Renderer::UnloadTexture(m_Framebuffer.texture.id);
    Renderer::UnloadRenderTexture(m_Framebuffer);
    m_Framebuffer = Renderer::LoadRenderTexture(m_Width, m_Height);
}
