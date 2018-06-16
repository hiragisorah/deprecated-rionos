#include "field-collision.h"
#include "camera.h"

void FieldCollision::Initialize(void)
{
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

	auto new_pos_z = position.z + 0.005f * z;

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
		transform.lock()->set_position({ position.x + x * 0.005f, new_height, new_pos_z + z * 0.005f });
}

void FieldCollision::Always(void)
{
}

unsigned int FieldCollision::GetHeightFromPos(DirectX::XMFLOAT3 pos)
{
	if (pos.x > +.95f)
		pos.x = +.95f;
	if (pos.x < -.95f)
		pos.x = -.95f;

	if (pos.y > +.95f)
		pos.y = +.95f;
	if (pos.y < -.95f)
		pos.y = -.95f;

	if (pos.z > +.95f)
		pos.z = +.95f;
	if (pos.z < -.95f)
		pos.z = -.95f;

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
	auto & texture = this->entity().lock()->scene().lock()->graphics()->texture_manager()->Get(this->field_);
	auto & pixels = texture->pixels_;
	auto & size = texture->size_;

	unsigned int x = (unsigned int)(size.x * uv.x);
	unsigned int y = (unsigned int)(size.y * uv.y);

	return pixels[x * (unsigned int)size.y + y];
}
