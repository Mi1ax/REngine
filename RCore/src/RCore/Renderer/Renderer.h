#pragma once

#include "RCore/Core/Base.h"
#include <glm/glm.hpp>

// Image, pixel data stored in CPU memory (RAM)
typedef struct Img {
    void* data;             // Image raw data
    int width;              // Image base width
    int height;             // Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Img;

// Raytangle, 4 components
typedef struct Rect {
    float x;                // Raytangle top-left corner position x
    float y;                // Raytangle top-left corner position y
    float width;            // Raytangle width
    float height;           // Raytangle height
} Rect;

class Renderer
{
public:
	static void Init(uint32_t width, uint32_t height, void* loader);
	static void Shutdown();

	static void Clear();
	static void SetClearColor(const glm::vec4& color);
	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
};
