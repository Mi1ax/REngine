#include "RCore/Core/Base.h"
#include "RCore/Core/Application.h"

class Sandbox2D : public Layer
{
public:
	Sandbox2D()
		: Layer("Sandbox2D")
	{
		
	}

	void OnAttach()
	{
		
	};
	void OnDetach()
	{
		
	};
	void OnUpdate(float dt)
	{
		
	};

	void OnImGuiRender()
	{
		
	};
};

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

int main()
{
	Scope<Sandbox> app = CreateScope<Sandbox>();
	app->Run();
	return 0;
}