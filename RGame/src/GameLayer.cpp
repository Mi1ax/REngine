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

    m_Scene = CreateRef<Scene>();
    m_RectEntity = m_Scene->CreateEntity("Rect");
    m_RectEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
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
        Renderer::SetViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
    }

    m_Framebuffer->Bind();
    Renderer::Clear();
	Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    m_Scene->OnUpdate(dt);
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
            if (m_RectEntity.HasComponent<TransformComponent>())
            {
                ImGui::DragFloat2("Position", glm::value_ptr(m_RectEntity.GetComponent<TransformComponent>().Position));
                ImGui::DragFloat2("Size", glm::value_ptr(m_RectEntity.GetComponent<TransformComponent>().Size));
            }
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
