#pragma once

#include <entt.hpp>


class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name);
	void DestroyEntity(Entity entity);

	void OnUpdate(float dt);
	void OnViewportResize(uint32_t width, uint32_t height);

private:
	entt::registry m_Registry;

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	friend class Entity;
};