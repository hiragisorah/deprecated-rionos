#pragma once

#include <iostream>

#include "..\seed-engine\component.h"

class TestComponent : public Seed::Component
{
public:
	TestComponent(std::shared_ptr<Component> & self)
		: Seed::Component(self)
	{

	}

private:
	void Initialize(void) override
	{

	}
	void Finalize(void) override {}
	void Pause(void) override {}
	void Update(void) override
	{
		//auto & graphics = this->entity().lock()->scene().lock()->
	}
	void Always(void) override
	{

	}
};