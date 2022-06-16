#include "rcpch.h"
#include "Input.h"

#include "Application.h"
#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	return { (float)xPos, (float)yPos };
}

float Input::GetMouseX()
{
	std::pair<float, float> pos = GetMousePosition();
	return pos.first;
}

float Input::GetMouseY()
{
	std::pair<float, float> pos = GetMousePosition();
	return pos.second;
}
