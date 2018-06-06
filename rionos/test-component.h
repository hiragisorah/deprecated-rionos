#pragma once

#include <iostream>

#include <DirectXMath.h>

#include "..\seed-engine\component.h"
#include "..\graphics.h"

constexpr float max_ = 2.f;

class TestComponent : public Seed::Component
{
public:
	TestComponent(std::shared_ptr<Component> & self)
		: Seed::Component(self)
	{
		
	}

private:
	std::shared_ptr<Renderer> field_;
	std::shared_ptr<Renderer> player_;
	DirectX::XMFLOAT3 pos_ = {};
	int sw = 1;
	struct CB
	{
		DirectX::XMMATRIX world_;
		DirectX::XMMATRIX view_;
		DirectX::XMMATRIX projection_;
	} cbuffer_, p_cbuffer_;

private:
	void Initialize(void) override
	{
		this->field_ = std::make_shared<Renderer>();
		this->player_ = std::make_shared<Renderer>();

		this->cbuffer_.world_ = DirectX::XMMatrixIdentity();
		this->cbuffer_.view_ = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 1.f, -1.f, 0), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
		this->cbuffer_.projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1920.f / 720.f, 0.1f, 1000.f);

		this->p_cbuffer_.world_ = DirectX::XMMatrixScaling(0.05f, 0.05f, 0.05f);
		this->p_cbuffer_.view_ = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 1.f, -1.f, 0), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
		this->p_cbuffer_.projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1920.f / 720.f, 0.1f, 1000.f);

		this->field_->constant_buffer_ = &this->cbuffer_;
		this->player_->constant_buffer_ = &this->p_cbuffer_;

		this->player_->draw_mode_ = DRAW_MODE_BACK_BUFFER_3D;
		this->player_->model_ = Resource::Model::PATH::kaoru;
		this->player_->shader_ = Resource::Shader::PATH::shader_3d;

		this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->field_);
		this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->player_);
	}
	void Finalize(void) override {}
	void Pause(void) override {}
	void Update(void) override
	{
		if (fabsf(this->pos_.x) >= 0.95f)
			sw *= -1;

		this->pos_.x += 0.005f * sw;
		this->pos_.y = (float)GetHeightFromPos(this->pos_) / 1000.f;
		this->p_cbuffer_.world_ = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f) * DirectX::XMMatrixTranslation(this->pos_.x, this->pos_.y, this->pos_.z);
	}
	void Always(void) override
	{

	}
	unsigned int GetHeightFromPos(DirectX::XMFLOAT3 pos)
	{
		return GetHeightFromUV(GetUvFromPos(pos));
	}
	DirectX::XMFLOAT2 GetUvFromPos(DirectX::XMFLOAT3 pos)
	{
		DirectX::XMFLOAT2 ret;

		ret.x = pos.x / max_ + 0.5f;
		ret.y = pos.z / max_ + 0.5f;

		return ret;
	}
	unsigned int GetHeightFromUV(DirectX::XMFLOAT2 uv)
	{
		auto & texture = this->entity().lock()->scene().lock()->graphics()->GetTexture(Resource::Texture::PATH::Simple);
		auto & pixels = texture->pixels_;
		auto & size = texture->size_;

		unsigned int x = (unsigned int)(size.x * uv.x);
		unsigned int y = (unsigned int)(size.y * uv.y);

		return pixels[x * (unsigned int)size.y + y];
	}
};