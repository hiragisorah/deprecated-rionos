#pragma once

#include <iostream>

#include "..\seed-engine\entity.h"

#include "test-component.h"

class TestEntity : public Seed::Entity
{
public:
	TestEntity(std::shared_ptr<Entity> & self)
		: Seed::Entity(self)
	{}

private:
	void Initialize(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
		this->AddComponent<TestComponent>();
	}
	void Finalize(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};