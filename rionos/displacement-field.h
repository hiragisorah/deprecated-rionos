#pragma once

#include <iostream>

#include <DirectXMath.h>

#include "..\seed-engine\scene.h"

class DisplacementField : public Seed::Component
{
public:
	DisplacementField(std::shared_ptr<Component> & self)
		: Seed::Component(self)
	{

	}

private:
	std::shared_ptr<Renderer> field_;

	struct CB
	{
		DirectX::XMMATRIX world_;
		DirectX::XMMATRIX view_;
		DirectX::XMMATRIX projection_;
	} cbuffer_;

private:
	void Initialize(void) override;
	void Finalize(void) override;
	void Pause(void) override;
	void Update(void) override;
	void Always(void) override;
};