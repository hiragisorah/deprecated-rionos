#include "transform.h"

const DirectX::XMFLOAT3 & Transform::position(void) const
{
	return this->position_;
}

const DirectX::XMFLOAT3 & Transform::rotation(void) const
{
	return this->rotation_;
}

const DirectX::XMFLOAT3 & Transform::scale(void) const
{
	return this->scale_;
}

void Transform::set_position(DirectX::XMFLOAT3 position)
{
	this->position_ = position;
}

void Transform::set_rotation(DirectX::XMFLOAT3 rotation)
{
	this->rotation_ = rotation;
}

void Transform::set_scale(DirectX::XMFLOAT3 scale)
{
	this->scale_ = scale;
}

DirectX::XMMATRIX Transform::GetTransformMatrix(void)
{
	return DirectX::XMMatrixScaling(this->scale_.x, this->scale_.y, this->scale_.z)
		* DirectX::XMMatrixRotationRollPitchYaw(this->rotation_.y, this->rotation_.z, this->rotation_.x)
		* DirectX::XMMatrixTranslation(this->position_.x, this->position_.y, this->position_.z);
}
