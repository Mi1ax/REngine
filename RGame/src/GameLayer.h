#pragma once

#include "RCore.h"
#include "ParticalSystem.h"

class GameLayer : public Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;

	void OnImGuiRender() override;

private:
	Ref<Framebuffer> m_Framebuffer;

	Ref<Scene> m_Scene;

	Entity m_RectEntity;

	glm::vec2 m_ViewportSize {};
	bool m_ViewportFocused = false, m_ViewportHovered = false;
};
