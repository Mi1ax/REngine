#pragma once

class ImGuiRender
{
public:
	static void Init();
	static void Destroy();

	static void BeginFrame();
	static void EndFrame();
};
