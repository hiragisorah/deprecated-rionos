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

		this->p_cbuffer_.world_ = DirectX::XMMatrixScaling(0.015f, 0.015f, 0.015f);
		this->p_cbuffer_.view_ = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 1.f, -1.f, 0), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
		this->p_cbuffer_.projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1920.f / 720.f, 0.1f, 1000.f);

		this->field_->constant_buffer_ = &this->cbuffer_;
		this->player_->constant_buffer_ = &this->p_cbuffer_;

		this->field_->rasterizer_state_ = RASTERIZER_STATE_WIREFRAME;


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
		auto x = -static_cast<int>(GetKeyState(VK_LEFT) < 0) + static_cast<int>(GetKeyState(VK_RIGHT) < 0);
		auto z = -static_cast<int>(GetKeyState(VK_DOWN) < 0) + static_cast<int>(GetKeyState(VK_UP) < 0);

		auto new_pos_x = this->pos_.x + 0.005f * x;

		if (new_pos_x > .95f)
			new_pos_x = .95f;
		if (new_pos_x < -.95f)
			new_pos_x = -.95f;

		auto new_pos_z = this->pos_.z + 0.005f * z;

		if (new_pos_z > .95f)
			new_pos_z = .95f;
		if (new_pos_z < -.95f)
			new_pos_z = -.95f;

		DirectX::XMFLOAT3 new_pos = { new_pos_x, 0, new_pos_z };

		auto new_height = (float)GetHeightFromPos(new_pos) / 1000.f;

		auto now_height = (float)GetHeightFromPos(this->pos_) / 1000.f;
		auto left_height = (float)GetHeightFromPos(DirectX::XMFLOAT3(this->pos_.x + 0.01f, 0, this->pos_.z)) / 1000.f;
		auto right_height = (float)GetHeightFromPos(DirectX::XMFLOAT3(this->pos_.x - 0.01f, 0, this->pos_.z)) / 1000.f;

		if (now_height > left_height && now_height < right_height)
		{
			new_height = left_height;
		}
		if (now_height < left_height && now_height > right_height)
		{
			new_height = right_height;
		}

		if (fabsf(this->pos_.y - new_height) < 0.1f)
		{
			this->pos_.y = new_height;
			this->pos_.x = new_pos_x;
			this->pos_.z = new_pos_z;
		}

		this->p_cbuffer_.world_ = DirectX::XMMatrixScaling(0.015f, 0.015f, 0.015f) * DirectX::XMMatrixTranslation(this->pos_.x, this->pos_.y, this->pos_.z);
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