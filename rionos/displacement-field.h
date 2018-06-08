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
	void Initialize(void) override
	{
		this->field_ = std::make_shared<Renderer>();

		this->field_->blend_state_ = BLEND_STATE_OPAQUE;
		this->field_->draw_mode_ = DRAW_MODE_BACK_BUFFER_DISP;
		this->field_->rasterizer_state_ = RASTERIZER_STATE_CCW;
		this->field_->sampler_state_ = SAMPLER_STATE_POINT_WRAP;
		this->field_->shader_ = ShaderPath::tesselation;
		this->field_->texture_2d_ = { TexturePath::Simple, TexturePath::Simple_normal };
		this->field_->constant_buffer_ = &this->cbuffer_;

		this->cbuffer_.world_ = DirectX::XMMatrixIdentity();
		this->cbuffer_.view_ = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 1.f, -1.f, 0), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
		this->cbuffer_.projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1920.f / 720.f, 0.1f, 1000.f);

		this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->field_);
	}
	void Finalize(void) override {}
	void Pause(void) override {}
	void Update(void) override {}
	void Always(void) override {}
};