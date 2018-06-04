#pragma once

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include "graphics.h"
#include "seed-engine\scene.h"

class Game
{
public:
	Game(Graphics * const graphics)
		: graphics_(graphics)
	{}

private:
	Graphics * const graphics_;

public:
	Graphics * const graphics(void) const { return this->graphics_; };

public:
	virtual void Initialize(void) = 0;
	virtual void Finalize(void) = 0;

public:
	virtual bool Run(void) = 0;

public:
	virtual void Destroy(void) = 0;
};