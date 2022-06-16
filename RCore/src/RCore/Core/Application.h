#pragma once

#include "Base.h"
#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"

#include "RCore/ImGui/ImGuiLayer.h"
#include "RCore/Event/ApplicationEvent.h"

class Application
{
public:
	Application(const std::string& title = "GCore", uint32_t width = 1280, uint32_t height = 720);
	~Application();

	void Run();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }

	inline Window& GetWindow() { return *m_Window; }
	inline void Close() { m_Running = false; }

	inline static Application& Get() { return *s_Instance; }

private:
	bool OnWindowClose(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);

private:
	Scope<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;

	bool m_Running = true;
	bool m_Minimazed = false;
	float m_LastFrameTime = 0.0f;

	LayerStack m_LayerStack;

private:
	static Application* s_Instance;
};
