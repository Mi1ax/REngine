﻿#include "rcpch.h"
#include "Renderer2D.h"

#include <rlgl.h>

#include <GLFW/glfw3.h>


void Renderer2D::BeginFrame()
{

}

void Renderer2D::EndFrame()
{
	rlDrawRenderBatchActive();
}

void Renderer2D::DrawRectangle(const Rect& rec, const glm::vec2& origin, float rotation, const glm::vec4& color)
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
        float sinRotation = glm::sin(glm::radians(rotation));
        float cosRotation = glm::cos(glm::radians(rotation));
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

    rlColor4f(color.r, color.g, color.b, color.a);

    rlVertex2f(topLeft.x, topLeft.y);
    rlVertex2f(bottomLeft.x, bottomLeft.y);
    rlVertex2f(topRight.x, topRight.y);

    rlVertex2f(topRight.x, topRight.y);
    rlVertex2f(bottomLeft.x, bottomLeft.y);
    rlVertex2f(bottomRight.x, bottomRight.y);

    rlEnd();
}

// Texture rendering
void Renderer2D::DrawTexture(const Ref<Texture> texture, Rect source, Rect dest, const glm::vec2& origin,
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
            float sinRotation = glm::sin(glm::radians(rotation));
            float cosRotation = glm::cos(glm::radians(rotation));
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

        rlColor4f(tintColor.r, tintColor.g, tintColor.b, tintColor.a);
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

void Renderer2D::DrawTexture(const Ref<Texture> texture, const glm::vec2& tiling, const glm::vec2& offset, const Rect& quad, const glm::vec4& tint)
{
    // WARNING: This solution only works if TEXTURE_WRAP_REPEAT is supported,
    // NPOT textures supported is required and OpenGL ES 2.0 could not support it
    Rect source = { offset.x * texture->GetWidth(), offset.y * texture->GetHeight(), tiling.x * texture->GetWidth(), tiling.y * texture->GetHeight()};

    DrawTexture(texture, source, quad, { 0.0f, 0.0f }, 0.0f, tint);
}

void Renderer2D::DrawSprite(const SpriteRendererComponent& src, const TransformComponent& tc)
{
    if (src.SpriteTexture)
        DrawTexture(
            src.SpriteTexture,
            { src.TilingFactor, src.TilingFactor },
            { 0.0f, 0.0f },
            { tc.Position.x, tc.Position.y, tc.Size.x, tc.Size.y },
            src.Color);
    else
        DrawRectangle({ tc.Position.x, tc.Position.y, tc.Size.x, tc.Size.y }, { 0.0f, 0.0f }, 0.0f, src.Color);
}
