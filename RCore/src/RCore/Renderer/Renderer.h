#pragma once

#include <glm/glm.hpp>

typedef struct Texture {
    unsigned int id;        // OpenGL texture id
    int width;              // Texture base width
    int height;             // Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Texture;

// RenderTexture, fbo for texture rendering
typedef struct RenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    Texture texture;        // Color buffer attachment texture
    Texture depth;          // Depth buffer attachment texture
} RenderTexture;

// Rectangle, 4 components
typedef struct Rect {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
} Rect;

class Renderer
{
public:
	static void Init(uint32_t width, uint32_t height, void* loader);
	static void Shutdown();

    static void Clear();
    static void SetClearColor(const glm::vec4& color);
	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	static void BeginFrame();
	static void EndFrame();

    // TODO: LoadTexture
    static void UnloadTexture(uint32_t textureID);

	static RenderTexture LoadRenderTexture(uint32_t width, uint32_t height);
    static void UnloadRenderTexture(RenderTexture renderTexture);

    static void BindFramebuffer(uint32_t frambufferID);
    static void UnbindFramebuffer();

    // Rendering Shapes
    static void DrawRectangle(const Rect& rec, const glm::vec2& origin, float rotation, const glm::vec4& color);
};
