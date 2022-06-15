#include "Framebuffer.h"

#include <iostream>
#include "rlgl.h"

static const uint32_t s_MaxFramebufferSize = 8192;

Framebuffer::Framebuffer(int width, int height)
	: m_Width(width), m_Height(height)
{
    LoadBuffer();
}

Framebuffer::~Framebuffer()
{
    rlUnloadTexture(m_Framebuffer.texture.id);
    rlUnloadFramebuffer(m_Framebuffer.id);
}

void Framebuffer::Bind()
{
    rlEnableFramebuffer(m_Framebuffer.id);
}

void Framebuffer::Unbind()
{
    rlDisableFramebuffer();
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
        // TODO: Logging System
        TRACELOG("Attempted to resize framebuffer to {0}, {1}", width, height);
        return;
    }

    m_Width = width;
    m_Height = height;
    LoadBuffer();
}


void Framebuffer::LoadBuffer()
{
    if (m_Framebuffer.id > 0)
    {
        rlUnloadTexture(m_Framebuffer.texture.id);
        rlUnloadFramebuffer(m_Framebuffer.id);
    }

    m_Framebuffer.id = rlLoadFramebuffer(m_Width, m_Height);   // Load an empty framebuffer

    if (m_Framebuffer.id > 0)
    {
        rlEnableFramebuffer(m_Framebuffer.id);

        // Create color texture (default to RGBA)
        m_Framebuffer.texture.id = rlLoadTexture(nullptr, m_Width, m_Height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_Framebuffer.texture.width = m_Width;
        m_Framebuffer.texture.height = m_Height;
        m_Framebuffer.texture.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_Framebuffer.texture.mipmaps = 1;

        // Create depth renderbuffer/texture
        m_Framebuffer.depth.id = rlLoadTextureDepth(m_Width, m_Height, true);
        m_Framebuffer.depth.width = m_Width;
        m_Framebuffer.depth.height = m_Height;
        m_Framebuffer.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        m_Framebuffer.depth.mipmaps = 1;

        // Attach color texture and depth renderbuffer/texture to FBO
        rlFramebufferAttach(m_Framebuffer.id, m_Framebuffer.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_Framebuffer.id, m_Framebuffer.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

        // Check if fbo is complete with attachments (valid)
        // TODO: Logging System
        if (rlFramebufferComplete(m_Framebuffer.id))
            TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", m_Framebuffer.id);

        rlDisableFramebuffer();
    }
    else // TODO: Logging System
        TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");
}
