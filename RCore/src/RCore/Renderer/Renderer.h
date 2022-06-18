#pragma once

#include <glm/glm.hpp>

#include "RCore/Renderer/Texture.h"

// Image, pixel data stored in CPU memory (RAM)
typedef struct Img {
    void* data;             // Image raw data
    int width;              // Image base width
    int height;             // Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Img;

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

    // Rendering Shapes
    static void DrawRectangle(const Rect& rec, const glm::vec2& origin, float rotation, const glm::vec4& color);

    // Texture rendering
    static void DrawTexture(
        const Ref<Texture> texture,
        Rect source, Rect dest, 
        const glm::vec2& origin, float rotation, 
        const glm::vec4& tintColor);
};
