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
	void Initialize(void) override
	{
		auto & transform = this->entity().lock()->GetComponent<Transform>();
		
		transform.lock()->set_scale(DirectX::XMFLOAT3(0.015f, 0.015f, 0.015f));

		this->player_ = std::make_shared<Renderer>();

		this->p_cbuffer_.world_ = transform.lock()->GetTransformMatrix();
		this->p_cbuffer_.view_ = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 1.f, -1.f, 0), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
		this->p_cbuffer_.projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1920.f / 720.f, 0.1f, 1000.f);

		this->player_->constant_buffer_ = &this->p_cbuffer_;

		this->player_->draw_mode_ = DRAW_MODE_BACK_BUFFER_3D;
		this->player_->model_ = ModelPath::kaoru;
		this->player_->shader_ = ShaderPath::shader_3d;

		this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->player_);
	}
	void Finalize(void) override {}
	void Pause(void) override {}
	void Update(void) override
	{
		auto & transform = this->entity().lock()->GetComponent<Transform>();
		auto & position = transform.lock()->position();

		auto x = -static_cast<int>(GetKeyState(VK_LEFT) < 0) + static_cast<int>(GetKeyState(VK_RIGHT) < 0);
		auto z = -static_cast<int>(GetKeyState(VK_DOWN) < 0) + static_cast<int>(GetKeyState(VK_UP) < 0);

		auto new_pos_x = position.x + 0.005f * x;

		if (new_pos_x > .95f)
			new_pos_x = .95f;
		if (new_pos_x < -.95f)
			new_pos_x = -.95f;

		auto new_pos_z = position.z + 0.005f * z;

		if (new_pos_z > .95f)
			new_pos_z = .95f;
		if (new_pos_z < -.95f)
			new_pos_z = -.95f;

		DirectX::XMFLOAT3 new_pos = { new_pos_x, 0, new_pos_z };

		auto now_height = (float)GetHeightFromPos(position) / 1000.f;
		auto left_height = (float)GetHeightFromPos(DirectX::XMFLOAT3(position.x + 0.01f, 0, position.z)) / 1000.f;
		auto right_height = (float)GetHeightFromPos(DirectX::XMFLOAT3(position.x - 0.01f, 0, position.z)) / 1000.f;

		if (now_height > left_height && now_height < right_height)
			new_pos_x += 0.005f * fabsf(left_height - right_height);

		if (now_height < left_height && now_height > right_height)
			new_pos_x -= 0.005f * fabsf(left_height - right_height);

		auto down_height = (float)GetHeightFromPos(DirectX::XMFLOAT3(position.x, 0, position.z + 0.01f)) / 1000.f;
		auto up_height = (float)GetHeightFromPos(DirectX::XMFLOAT3(position.x, 0, position.z - 0.01f)) / 1000.f;

		if (now_height > down_height && now_height < up_height)
			new_pos_z += 0.005f * fabsf(down_height - up_height);

		if (now_height < down_height && now_height > up_height)
			new_pos_z -= 0.005f * fabsf(down_height - up_height);

		auto new_height = (float)GetHeightFromPos(new_pos) / 1000.f;

		if (fabsf(position.y - new_height) < 0.09f)
			transform.lock()->set_position({ new_pos_x, new_height, new_pos_z });

		this->p_cbuffer_.world_ = DirectX::XMMatrixScaling(0.015f, 0.015f, 0.015f) * DirectX::XMMatrixTranslation(position.x, position.y, position.z);
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
		auto & texture = this->entity().lock()->scene().lock()->graphics()->GetTexture(this->field_);
		auto & pixels = texture->pixels_;
		auto & size = texture->size_;

		unsigned int x = (unsigned int)(size.x * uv.x);
		unsigned int y = (unsigned int)(size.y * uv.y);

		return pixels[x * (unsigned int)size.y + y];
	}
};