#include "field-collision.h"
#include "camera.h"

void FieldCollision::Initialize(void)
{
	auto & transform = this->entity().lock()->GetComponent<Transform>();
	auto & camera = this->entity().lock()->scene().lock()->GetSystem<Camera>();

	transform.lock()->set_scale(DirectX::XMFLOAT3(0.015f, 0.015f, 0.015f));

	this->player_ = std::make_shared<Renderer>();

	this->p_cbuffer_.world_ = transform.lock()->GetTransformMatrix();
	this->p_cbuffer_.view_ = camera.lock()->view();
	this->p_cbuffer_.projection_ = camera.lock()->projection();

	this->player_->constant_buffer_ = &this->p_cbuffer_;

	this->player_->draw_mode_ = DRAW_MODE_BACK_BUFFER_3D;
	this->player_->model_ = ModelPath::kaoru;
	this->player_->shader_ = ShaderPath::shader_3d;

	this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->player_);
}

void FieldCollision::Finalize(void)
{
}

void FieldCollision::Pause(void)
{
}

void FieldCollision::Update(void)
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

void FieldCollision::Always(void)
{
}

unsigned int FieldCollision::GetHeightFromPos(DirectX::XMFLOAT3 pos)
{
	return GetHeightFromUV(GetUvFromPos(pos));
}

DirectX::XMFLOAT2 FieldCollision::GetUvFromPos(DirectX::XMFLOAT3 pos)
{
	DirectX::XMFLOAT2 ret;

	ret.x = pos.x / max_ + 0.5f;
	ret.y = pos.z / max_ + 0.5f;

	return ret;
}

unsigned int FieldCollision::GetHeightFromUV(DirectX::XMFLOAT2 uv)
{
	auto & texture = this->entity().lock()->scene().lock()->graphics()->GetTexture(this->field_);
	auto & pixels = texture->pixels_;
	auto & size = texture->size_;

	unsigned int x = (unsigned int)(size.x * uv.x);
	unsigned int y = (unsigned int)(size.y * uv.y);

	return pixels[x * (unsigned int)size.y + y];
}
