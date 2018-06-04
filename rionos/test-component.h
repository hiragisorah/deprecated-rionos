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
		std::cout << __FUNCTION__ << std::endl;
	}
	void Finalize(void) override {}
	void Pause(void) override {}
	void Update(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
	void Always(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};