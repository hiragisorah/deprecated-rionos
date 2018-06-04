#pragma once

#include "..\game.h"

class __declspec(dllexport) Rionos : public Game
{
public:
	void Initialize(void) override;
	bool Run(void) override;
	void Finalize(void) override;

public:
	void Destroy(void) override;
};

Game __declspec(dllexport) * Create(void);