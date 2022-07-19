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
        color.r,
        color.g,
        color.b,
        color.a
    );
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    rlViewport(x, y, width, height);
    rlMatrixMode(RL_PROJECTION);                            // Switch to PROJECTION matrix
    rlLoadIdentity();                                       // Reset current matrix (PROJECTION)
    rlOrtho(x, width, height, y, 0.0f, 1.0f);               // Orthographic projection with top-left corner at (0,0)
}
