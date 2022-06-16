#include "rcpch.h"
#include "Renderer.h"

#define RLGL_IMPLEMENTATION
#define GRAPHICS_API_OPENGL_33
#include <rlgl.h>

void Renderer::Init(uint32_t width, uint32_t height, void* loader)
{
	rlLoadExtensions(loader);

	rlglInit(width, height);
    // Initialize viewport and internal projection/modelview matrices
    rlViewport(0, 0, width, height);
    rlMatrixMode(RL_PROJECTION);                            // Switch to PROJECTION matrix
    rlLoadIdentity();                                       // Reset current matrix (PROJECTION)
    rlOrtho(0, width, height, 0, 0.0f, 1.0f);               // Orthographic projection with top-left corner at (0,0)
    rlMatrixMode(RL_MODELVIEW);                             // Switch back to MODELVIEW matrix
    rlLoadIdentity();                                       // Reset current matrix (MODELVIEW)
    rlEnableDepthTest();
}

void Renderer::Shutdown()
{
	rlglClose();
}

void Renderer::Clear()
{
    rlClearScreenBuffers();
}

void Renderer::SetClearColor(const glm::vec4& color)
{
    rlClearColor(
        color.r * 255,
        color.g * 255,
        color.b * 255,
        color.a * 255
        );
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    rlViewport(x, y, width, height);
    rlMatrixMode(RL_PROJECTION);                            // Switch to PROJECTION matrix
    rlLoadIdentity();                                       // Reset current matrix (PROJECTION)
    rlOrtho(x, width, height, y, 0.0f, 1.0f);               // Orthographic projection with top-left corner at (0,0)
}

void Renderer::BeginFrame()
{

}

void Renderer::EndFrame()
{
	rlDrawRenderBatchActive();
}

void Renderer::UnloadTexture(uint32_t textureID)
{
    rlUnloadTexture(textureID);
}

void Renderer::UnloadRenderTexture(RenderTexture renderTexture)
{
    rlUnloadFramebuffer(renderTexture.id);
}

void Renderer::BindFramebuffer(uint32_t frambufferID)
{
    rlEnableFramebuffer(frambufferID);
}

void Renderer::UnbindFramebuffer()
{
    rlDisableFramebuffer();
}

RenderTexture Renderer::LoadRenderTexture(uint32_t width, uint32_t height)
{
    RenderTexture renderTexture = {};

    if (renderTexture.id > 0)
    {
        rlUnloadTexture(renderTexture.texture.id);
        rlUnloadFramebuffer(renderTexture.id);
    }

    renderTexture.id = rlLoadFramebuffer(width, height);   // Load an empty framebuffer

    if (renderTexture.id > 0)
    {
        rlEnableFramebuffer(renderTexture.id);

        // Create color texture (default to RGBA)
        renderTexture.texture.id = rlLoadTexture(nullptr, width, height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        renderTexture.texture.width = width;
        renderTexture.texture.height = height;
        renderTexture.texture.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        renderTexture.texture.mipmaps = 1;

        // Create depth renderbuffer/texture
        renderTexture.depth.id = rlLoadTextureDepth(width, height, true);
        renderTexture.depth.width = width;
        renderTexture.depth.height = height;
        renderTexture.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        renderTexture.depth.mipmaps = 1;

        // Attach color texture and depth renderbuffer/texture to FBO
        rlFramebufferAttach(renderTexture.id, renderTexture.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(renderTexture.id, renderTexture.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
        
        rlDisableFramebuffer();
    }
    else
        CORE_ERROR("FBO: Framebuffer object can not be created");

    return renderTexture;
}

void Renderer::DrawRectangle(const Rect& rec, const glm::vec2& origin, float rotation, const glm::vec4& color)
{
    glm::vec2 topLeft = { 0.0f, 0.0f };
    glm::vec2 topRight = { 0.0f, 0.0f };
    glm::vec2 bottomLeft = { 0.0f, 0.0f };
    glm::vec2 bottomRight = { 0.0f, 0.0f };

    // Only calculate rotation if needed
    if (rotation == 0.0f)
    {
        float x = rec.x - origin.x;
        float y = rec.y - origin.y;
        topLeft = { x, y };
        topRight = { x + rec.width, y };
        bottomLeft = { x, y + rec.height };
        bottomRight = { x + rec.width, y + rec.height };
    }
    else
    {
        float sinRotation = sinf(rotation * DEG2RAD);
        float cosRotation = cosf(rotation * DEG2RAD);
        float x = rec.x;
        float y = rec.y;
        float dx = -origin.x;
        float dy = -origin.y;

        topLeft.x = x + dx * cosRotation - dy * sinRotation;
        topLeft.y = y + dx * sinRotation + dy * cosRotation;

        topRight.x = x + (dx + rec.width) * cosRotation - dy * sinRotation;
        topRight.y = y + (dx + rec.width) * sinRotation + dy * cosRotation;

        bottomLeft.x = x + dx * cosRotation - (dy + rec.height) * sinRotation;
        bottomLeft.y = y + dx * sinRotation + (dy + rec.height) * cosRotation;

        bottomRight.x = x + (dx + rec.width) * cosRotation - (dy + rec.height) * sinRotation;
        bottomRight.y = y + (dx + rec.width) * sinRotation + (dy + rec.height) * cosRotation;
    }
    
    rlCheckRenderBatchLimit(6);

    rlBegin(RL_TRIANGLES);

    rlColor4ub(color.r * 255, color.g * 255, color.b * 255, color.a * 255);

    rlVertex2f(topLeft.x, topLeft.y);
    rlVertex2f(bottomLeft.x, bottomLeft.y);
    rlVertex2f(topRight.x, topRight.y);

    rlVertex2f(topRight.x, topRight.y);
    rlVertex2f(bottomLeft.x, bottomLeft.y);
    rlVertex2f(bottomRight.x, bottomRight.y);

    rlEnd();
}
