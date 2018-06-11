#pragma once

#include <iostream>

#include <DirectXMath.h>

#include "..\seed-engine\scene.h"
#include "transform.h"

constexpr float max_ = 2.f;

class PlayerMovement : public Seed::Component
{
public:
	PlayerMovement(std::shared_ptr<Component> & self)
		: Seed::Component(self)
	{

	}

private:
	void Initialize(void) override;
	void Finalize(void) override;
	void Pause(void) override;
	void Update(void) override;
	void Always(void) override;
};