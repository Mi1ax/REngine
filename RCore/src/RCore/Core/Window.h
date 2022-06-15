#pragma once
#include "Base.h"
#include "Framebuffer.h"

#include <iostream>

struct GLFWwindow;

class Window
{
public:
	Window(const char* title, uint32_t width, uint32_t height);
	~Window();

	void Run();

	Ref<Framebuffer> GetFramebuffer() const { return m_Framebuffer; }
	GLFWwindow* GetNativeWindow() const { return m_Window; }

	uint32_t GetWidth() const { return m_Data.Width; }
	uint32_t GetHeight() const { return m_Data.Height; }

	static Window& Get() { return *s_Instance; }

private:
	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
	};
	WindowData m_Data;

	GLFWwindow* m_Window;

	Ref<Framebuffer> m_Framebuffer;

	Vector2 m_ViewportSize{};

private:
	static Window* s_Instance;
};