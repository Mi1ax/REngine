#include "RCore/Core/Base.h"
#include "RCore/Core/Window.h"

int main()
{
	Scope<Window> window = CreateScope<Window>("Window", 1280, 720);
	window->Run();
	return 0;
}