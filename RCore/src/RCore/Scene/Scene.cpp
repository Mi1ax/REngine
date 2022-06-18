#include "rcpch.h"
#include "Scene.h"

#include "rcpch.h"
#include "Entity.h"
#include "Components.h"

#include "RCore/Renderer/Renderer.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), this };

	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;

	entity.AddComponent<TransformComponent>();

	return entity;
}

void Scene::DestroyEntity(Entity entity)
{
	m_Registry.destroy(entity);
}

void Scene::OnUpdate(float dt)
{
	Renderer::BeginFrame();

	auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
	for (auto entity : group)
	{
		auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		
		Renderer::DrawSprite(sprite, transform);
	}

	Renderer::EndFrame();
}

void Scene::OnViewportResize(uint32_t width, uint32_t height)
{
	
}