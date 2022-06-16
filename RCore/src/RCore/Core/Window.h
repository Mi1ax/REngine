#pragma once
#include "Base.h"

#include "Framebuffer.h"
#include "LayerStack.h"

#include <iostream>

struct GLFWwindow;
typedef void (*GLFWwindowclosefun)(GLFWwindow*);

class Window
{
public:
	Window(const std::string& title, uint32_t width, uint32_t height);
	~Window() = default;

	void OnUpdate();

	Ref<Framebuffer> GetFramebuffer() const { return m_Framebuffer; }

	uint32_t GetWidth() const { return m_Data.Width; }
	uint32_t GetHeight() const { return m_Data.Height; }

	GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
	void Init();
	void Shutdown();

private:
	GLFWwindow* m_Window;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
	};
	WindowData m_Data {};

	Ref<Framebuffer> m_Framebuffer;
	Vector2 m_ViewportSize{};
};