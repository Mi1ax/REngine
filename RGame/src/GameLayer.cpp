#include <RCore.h>
#include <imgui.h>

#include "GameLayer.h"
#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GameLayer::GameLayer()
	: Layer("GameLayer") {}

void GameLayer::OnAttach()
{
	m_Framebuffer = CreateRef<Framebuffer>(1280, 720);

    // Init here
    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 25.0f, m_Particle.SizeVariation = 15.0f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 450.0f, 250.0f };
    m_Particle.Position = { 0.0f, 0.0f };
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(float dt)
{
    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
        ((float)m_Framebuffer->GetWidth() != m_ViewportSize.x || (float)m_Framebuffer->GetHeight() != m_ViewportSize.y))
    {
        m_Framebuffer->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
    }

    m_Framebuffer->Bind();
    Renderer::Clear();
	Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Renderer::BeginFrame();
	{
        if (Input::IsMouseButtonPressed(1))
        {
            std::pair<float, float> mPos = Input::GetMousePosition();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            
            m_Particle.Position = { mPos.first, mPos.second };
            for (int i = 0; i < 5; i++)
                m_ParticleSystem.Emit(m_Particle);
        }

        m_ParticleSystem.OnUpdate(dt);
        m_ParticleSystem.OnRender();
	}
	Renderer::EndFrame();
    m_Framebuffer->Unbind();
}

void GameLayer::OnImGuiRender()
{
    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    {
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");
        {
            ImGui::ColorEdit4("Color Begin", glm::value_ptr(m_Particle.ColorBegin));
            ImGui::ColorEdit4("Color End", glm::value_ptr(m_Particle.ColorEnd));
            ImGui::DragFloat("Size Begin", &m_Particle.SizeBegin);
            ImGui::DragFloat("Size Variation", &m_Particle.SizeVariation);
            ImGui::DragFloat("Life Time", &m_Particle.LifeTime);
            ImGui::DragFloat2("Velocity Variation", glm::value_ptr(m_Particle.VelocityVariation));
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        {
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

            ImVec2 size = ImGui::GetContentRegionAvail();
            m_ViewportSize = { size.x, size.y };

            ImGui::Image(
                (void*)m_Framebuffer->GetColorAttachmentRendererID(),
                size,
                ImVec2{ 0, 1 }, ImVec2{ 1, 0 }
            );
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
    ImGui::End();
}
