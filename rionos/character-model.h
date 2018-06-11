#pragma once

#include <iostream>

#include <DirectXMath.h>

#include "..\seed-engine\scene.h"
#include "transform.h"

class CharacterModel : public Seed::Component
{
public:
	CharacterModel(std::shared_ptr<Component> & self, const ModelPath & model)
		: Seed::Component(self)
		, model_(model)
	{

	}

private:
	std::shared_ptr<Renderer> player_;
	ModelPath model_;

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