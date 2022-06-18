#include "rcpch.h"
#include "Renderer.h"

#define RLGL_IMPLEMENTATION
#define GRAPHICS_API_OPENGL_33
#include <rlgl.h>

#include <GLFW/glfw3.h>

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

// Texture rendering
void Renderer::DrawTexture(const Ref<Texture> texture, Rect source, Rect dest, const glm::vec2& origin,
	float rotation, const glm::vec4& tintColor)
{
    // Check if texture is valid
    if (texture->GetID() > 0)
    {
        float width = (float)texture->GetWidth();
        float height = (float)texture->GetHeight();

        bool flipX = false;

        if (source.width < 0) { flipX = true; source.width *= -1; }
        if (source.height < 0) source.y -= source.height;

        glm::vec2 topLeft = { 0.0f, 0.0f };
        glm::vec2 topRight = { 0.0f, 0.0f };
        glm::vec2 bottomLeft = { 0.0f, 0.0f };
        glm::vec2 bottomRight = { 0.0f, 0.0f };

        // Only calculate rotation if needed
        if (rotation == 0.0f)
        {
            float x = dest.x - origin.x;
            float y = dest.y - origin.y;
            topLeft = { x, y };
            topRight = { x + dest.width, y };
            bottomLeft = { x, y + dest.height };
            bottomRight = { x + dest.width, y + dest.height };
        }
        else
        {
            float sinRotation = sinf(rotation * DEG2RAD);
            float cosRotation = cosf(rotation * DEG2RAD);
            float x = dest.x;
            float y = dest.y;
            float dx = -origin.x;
            float dy = -origin.y;

            topLeft.x = x + dx * cosRotation - dy * sinRotation;
            topLeft.y = y + dx * sinRotation + dy * cosRotation;

            topRight.x = x + (dx + dest.width) * cosRotation - dy * sinRotation;
            topRight.y = y + (dx + dest.width) * sinRotation + dy * cosRotation;

            bottomLeft.x = x + dx * cosRotation - (dy + dest.height) * sinRotation;
            bottomLeft.y = y + dx * sinRotation + (dy + dest.height) * cosRotation;

            bottomRight.x = x + (dx + dest.width) * cosRotation - (dy + dest.height) * sinRotation;
            bottomRight.y = y + (dx + dest.width) * sinRotation + (dy + dest.height) * cosRotation;
        }

        rlCheckRenderBatchLimit(4);     // Make sure there is enough free space on the batch buffer

        rlSetTexture(texture->GetID());
        rlBegin(RL_QUADS);

        rlColor4ub(tintColor.r * 255, tintColor.g * 255, tintColor.b * 255, tintColor.a * 255);
        rlNormal3f(0.0f, 0.0f, 1.0f);                          // Normal vector pointing towards viewer

        // Top-left corner for texture and quad
        if (flipX) rlTexCoord2f((source.x + source.width) / width, source.y / height);
        else rlTexCoord2f(source.x / width, source.y / height);
        rlVertex2f(topLeft.x, topLeft.y);

        // Bottom-left corner for texture and quad
        if (flipX) rlTexCoord2f((source.x + source.width) / width, (source.y + source.height) / height);
        else rlTexCoord2f(source.x / width, (source.y + source.height) / height);
        rlVertex2f(bottomLeft.x, bottomLeft.y);

        // Bottom-right corner for texture and quad
        if (flipX) rlTexCoord2f(source.x / width, (source.y + source.height) / height);
        else rlTexCoord2f((source.x + source.width) / width, (source.y + source.height) / height);
        rlVertex2f(bottomRight.x, bottomRight.y);

        // Top-right corner for texture and quad
        if (flipX) rlTexCoord2f(source.x / width, source.y / height);
        else rlTexCoord2f((source.x + source.width) / width, source.y / height);
        rlVertex2f(topRight.x, topRight.y);

        rlEnd();
        rlSetTexture(0);
    }
}
