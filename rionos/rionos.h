#pragma once

#include "..\game.h"

class __declspec(dllexport) Rionos : public Game
{
public:
	Rionos(Graphics * const graphics)
		: Game(graphics)
	{}

public:
	void Initialize(void) override;
	bool Run(void) override;
	void Finalize(void) override;

public:
	void Destroy(void) override;

private:
	std::shared_ptr<Seed::Scene> main_scene_;
};

Game __declspec(dllexport) * const Create(Graphics * const graphics);