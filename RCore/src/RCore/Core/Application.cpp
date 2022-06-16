#include "rcpch.h"
#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& title, uint32_t width, uint32_t height)
{
	Log::Init();

	CORE_ASSERT(!s_Instance, "Application is already exists!");
	s_Instance = this;

	m_Window = CreateScope<Window>(title, width, height);

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

Application::~Application() = default;

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
	overlay->OnAttach();
}

void Application::Run()
{
	while (m_Running)
	{
		float timestep = GetFrameTime();

		if (!m_Minimazed) {
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);
		}

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnUpdate();
	}
}