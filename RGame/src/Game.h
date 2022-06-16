#pragma once

#include "RCore.h"

class Game : public Application
{
public:
	Game()
	{
		PushLayer(new GameLayer());
	}

	~Game()
	{

	}
};