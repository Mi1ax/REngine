#include "rcpch.h"
#include "Window.h"

#include <imgui.h>
#include <rlgl.h>
#include <GLFW/glfw3.h>

#include "Framebuffer.h"
#include "raylib.h"

#include "RCore/Event/ApplicationEvent.h"

Window::Window(const std::string& title, uint32_t width, uint32_t height)
{
    SetTraceLogLevel(LOG_DEBUG | LOG_ERROR | LOG_WARNING | LOG_FATAL);

    m_Data.Title = title;
    m_Data.Width = width;
    m_Data.Height = height;

    Init();
    
    m_Framebuffer = CreateRef<Framebuffer>(m_Data.Width, m_Data.Height);
}

void Window::Init()
{
    glfwSetErrorCallback([](int error, const char* description)
        {
            CORE_ERROR("GLFW Error ({0}): {1}", error, description);
        });

    CORE_ASSERT(glfwInit(), "Can't initialize GLFW");

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

    if (!m_Window)
    {
        glfwTerminate();
        CORE_ERROR("GLFW3: Can't initialize window");
    }
    else CORE_INFO("GLFW3: Window created successfully");

    glfwSetWindowUserPointer(m_Window, &m_Data);

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        // TODO: Improve
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    });

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);

    rlLoadExtensions(glfwGetProcAddress);

    rlglInit(m_Data.Width, m_Data.Height);

    // Initialize viewport and internal projection/modelview matrices
    rlViewport(0, 0, m_Data.Width, m_Data.Height);
    rlMatrixMode(RL_PROJECTION);                        // Switch to PROJECTION matrix
    rlLoadIdentity();                                   // Reset current matrix (PROJECTION)
    rlOrtho(0, m_Data.Width, m_Data.Height, 0, 0.0f, 1.0f); // Orthographic projection with top-left corner at (0,0)
    rlMatrixMode(RL_MODELVIEW);                         // Switch back to MODELVIEW matrix
    rlLoadIdentity();                                   // Reset current matrix (MODELVIEW)

    rlClearColor(245, 245, 245, 255);                   // Define clear color
    rlEnableDepthTest();
}

void Window::Shutdown()
{
    rlglClose();

    glfwDestroyWindow(m_Window);
}

void Window::OnUpdate()
{
    /*bool open = true;
    while(!glfwWindowShouldClose(m_Window))
    {
        rlClearScreenBuffers();

        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            ((float)m_Framebuffer->GetWidth() != m_ViewportSize.x || (float)m_Framebuffer->GetHeight() != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
        }

        ImGuiRender::BeginFrame();
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
                            glfwSetWindowShouldClose(m_Window, true);
                        ImGui::EndMenu();
                    }

                    ImGui::EndMenuBar();
                }

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
                ImGui::Begin("Viewport");
                {
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
        ImGuiRender::EndFrame();

        BeginTextureMode(m_Framebuffer->GetTarget());
        {
            ClearBackground(PINK);
            DrawRectangleV({ 0.0f, 0.0f }, m_ViewportSize, BLACK);
        }
        EndTextureMode();*/

    glfwPollEvents();
    glfwSwapBuffers(m_Window);
	//}
}
