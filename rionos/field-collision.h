#pragma once

#include <iostream>

#include <DirectXMath.h>

#include "..\seed-engine\scene.h"
#include "transform.h"

constexpr float max_ = 2.f;

class FieldCollision : public Seed::Component
{
public:
	FieldCollision(std::shared_ptr<Component> & self, TexturePath field)
		: Seed::Component(self)
		, field_(field)
	{
		
	}

private:
	std::shared_ptr<Renderer> player_;
	TexturePath field_;

	struct CB
	{
		DirectX::XMMATRIX world_;
		DirectX::XMMATRIX view_;
		DirectX::XMMATRIX projection_;
	} p_cbuffer_;

private:
	void Initialize(void) override;
	void Finalize(void) override;
	void Pause(void) override;
	void Update(void) override;
	void Always(void) override;
	unsigned int GetHeightFromPos(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT2 GetUvFromPos(DirectX::XMFLOAT3 pos);
	unsigned int GetHeightFromUV(DirectX::XMFLOAT2 uv);
};