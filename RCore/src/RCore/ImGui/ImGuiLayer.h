#pragma once

#include "RCore/Core/Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void SetDarkThemeColors();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Event& e) override;

	void SetBlockEvents(bool block) { m_BlockEvents = block; }

	void Begin();
	void End();

private:
	bool m_BlockEvents = true;
};