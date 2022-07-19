#include <RCore.h>
#include "GameLayer.h"
#include "Game.h"

int main()
{
	Log::Init();
	Scope<Game> app = CreateScope<Game>();
	app->Run();
	return 0;
}