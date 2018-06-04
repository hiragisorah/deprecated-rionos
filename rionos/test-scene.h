#pragma once

#include <iostream>

#include "..\seed-engine\scene.h"

#include "test-entity.h"

class TestScene : public Seed::Scene
{
public:
	TestScene(std::shared_ptr<Scene> & self)
		: Seed::Scene(self)
	{

	}

private:
	void Initialize(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
		this->AddChild<TestEntity>();
	}
	void Finalize(void) override {}
};