#include "rcpch.h"
#include "Framebuffer.h"

#include <iostream>

#include "rlgl.h"

static const uint32_t s_MaxFramebufferSize = 8192;

Framebuffer::Framebuffer(int width, int height)
	: m_Width(width), m_Height(height)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    rlUnloadTexture(m_ColorAttachment);
    rlUnloadTexture(m_DepthAttachment);
    rlUnloadFramebuffer(m_RendererID);
}

void Framebuffer::Invalidate()
{
    m_RendererID = rlLoadFramebuffer(m_Width, m_Height);   // Load an empty framebuffer

    if (m_RendererID > 0)
    {
        rlEnableFramebuffer(m_RendererID);

        // Create color texture (default to RGBA)
        m_ColorAttachment = rlLoadTexture(nullptr, m_Width, m_Height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        /*renderTexture.texture.id =
        renderTexture.texture.width = width;
        renderTexture.texture.height = height;
        renderTexture.texture.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        renderTexture.texture.mipmaps = 1;*/

        // Create depth renderbuffer/texture
        m_DepthAttachment = rlLoadTextureDepth(m_Width, m_Height, true);
        /*renderTexture.depth.id = rlLoadTextureDepth(width, height, true);
        renderTexture.depth.width = width;
        renderTexture.depth.height = height;
        renderTexture.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        renderTexture.depth.mipmaps = 1;*/

        // Attach color texture and depth renderbuffer/texture to FBO
        rlFramebufferAttach(m_RendererID, m_ColorAttachment, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_RendererID, m_DepthAttachment, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

        rlDisableFramebuffer();
    }
    else
        CORE_ERROR("Framebuffer object can not be created");
}

void Framebuffer::Bind()
{
    rlEnableFramebuffer(m_RendererID);
}

void Framebuffer::Unbind()
{
    rlDisableFramebuffer();
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
    rlUnloadTexture(m_ColorAttachment);
    rlUnloadFramebuffer(m_RendererID);
    Invalidate();
}
