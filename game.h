#pragma once

class Game
{
public:
	virtual void Initialize(void) = 0;
	virtual void Finalize(void) = 0;

public:
	virtual bool Run(void) = 0;

public:
	virtual void Destroy(void) = 0;
};