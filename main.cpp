#include <iostream>
#include <string>
#include <functional>

#include <Windows.h>

#include "dll.h"
#include "window.h"
#include "graphics.h"
#include "game.h"

template<class ... Args> bool NullCheck(Args * ... args)
{
	bool ret = true;

	for (auto & p : { static_cast<void*>(args)... })
	{
		if (p == nullptr)
		{
			ret = false;
		}
	}

	return ret;
}

int main(void)
{
	DLLClass<Window> window;
	window.Create("Debug/simple-window.dll", std::string("window"), 1280U, 720U);

	DLLClass<Graphics> graphics;
	graphics.Create("Debug/directx11.dll", window);

	DLLClass<Game> game;
	game.Create("Debug/rionos.dll", graphics);

	if (!NullCheck(window.Class(), graphics.Class(), game.Class())) return 0;
	
	window->Initialize();
	graphics->Initialize();
	game->Initialize();

	while (window->Run() && graphics->Run() && game->Run());

	window->Finalize();
	graphics->Finalize();
	game->Finalize();
	
	window->Destroy();
	graphics->Destroy();
	game->Destroy();

	return 0;
}