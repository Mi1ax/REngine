#include <RCore.h>
#include "GameLayer.h"
#include "Game.h"

int main()
{
	Scope<Game> app = CreateScope<Game>();
	app->Run();
	return 0;
}