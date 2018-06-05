#pragma once

#include <iostream>

#include <DirectXMath.h>

#include "..\seed-engine\component.h"
#include "..\graphics.h"

class TestComponent : public Seed::Component
{
public:
	TestComponent(std::shared_ptr<Component> & self)
		: Seed::Component(self)
	{
		
	}

private:
	std::shared_ptr<Renderer> renderer_;
	struct CB
	{
		DirectX::XMMATRIX world_;
		DirectX::XMMATRIX view_;
		DirectX::XMMATRIX projection_;
	} cbuffer_;

private:
	void Initialize(void) override
	{
		this->renderer_ = std::make_shared<Renderer>();

		this->cbuffer_.world_ = DirectX::XMMatrixIdentity();
		this->cbuffer_.view_ = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 70.f, -70.f, 0), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
		this->cbuffer_.projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1920.f / 720.f, 0.1f, 1000.f);

		this->renderer_->constant_buffer_ = &this->cbuffer_;

		this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->renderer_);
	}
	void Finalize(void) override {}
	void Pause(void) override {}
	void Update(void) override
	{
	}
	void Always(void) override
	{

	}
};