#pragma once

#include "rcpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct TagComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag) : Tag(tag) {}
};

struct TransformComponent
{
	glm::vec2 Position{ 0.0f, 0.0f };
	glm::vec2 Size{ 128.0f, 128.0f };
	float Rotation = 0.0f;
	
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec2& translation) : Position(translation) {}
};

struct SpriteRendererComponent
{
	glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

	SpriteRendererComponent() = default;
	SpriteRendererComponent(const SpriteRendererComponent&) = default;
	SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
};