#include "rcpch.h"
#include "Application.h"

#include "RCore/Core/Base.h"
#include "RCore/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& title, uint32_t width, uint32_t height)
{
	CORE_ASSERT(!s_Instance, "Application is already exists!");
	s_Instance = this;

	m_Window = CreateScope<Window>(title, width, height);
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
}

Application::~Application() = default;

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		if (e.Handled)
			break;
		(*--it)->OnEvent(e);
	}
}

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

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_Minimazed = true;
		return false;
	}

	m_Minimazed = false;
	Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

	return false;
}

void Application::Run()
{
	while (m_Running)
	{
		float time = (float)glfwGetTime();
		m_Timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimazed) {
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(m_Timestep);
		}

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnUpdate();
	}
}