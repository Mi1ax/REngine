#pragma once

#include <glm/glm.hpp>

#include "RCore/Renderer/Renderer.h"
#include "RCore/Renderer/Texture.h"
#include "RCore/Scene/Components.h"

class Renderer2D
{
public:
	static void BeginFrame();
	static void EndFrame();

    // Shapes Rendering
    static void DrawRectangle(const Rect& rec, const glm::vec2& origin, float rotation, const glm::vec4& color);

    // Texture Rendering
    static void DrawTexture(
        const Ref<Texture> texture,
        Rect source, Rect dest, 
        const glm::vec2& origin, float rotation, 
        const glm::vec4& tintColor);

    static void DrawTexture(
        const Ref<Texture> texture, 
        const glm::vec2& tiling, 
        const glm::vec2& offset, 
        const Rect& quad, 
        const glm::vec4& tint);

    // Entity Rendering
    static void DrawSprite(const SpriteRendererComponent& src, const TransformComponent& tc);
};
