#include "camera.h"

Camera::Camera(std::shared_ptr<System>& self)
	: Seed::System(self)
	, eye_(DirectX::XMVectorSet(-1.f, 1.f, -1.f, 0.f))
	, at_(DirectX::XMVectorZero())
	, up_(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f))
	, fov_(DirectX::XM_PIDIV4)
	, aspect_(1920.f / 720.f)
	, near_(0.1f)
	, far_(1000.f)
{
}

DirectX::XMMATRIX Camera::view(void)
{
	return DirectX::XMMatrixLookAtLH(this->eye_, this->at_, this->up_);
}

DirectX::XMMATRIX Camera::projection(void)
{
	return DirectX::XMMatrixPerspectiveFovLH(this->fov_, this->aspect_, this->near_, this->far_);
}

void Camera::Initalize(void)
{
}

void Camera::Finalize(void)
{
}

void Camera::Pause(void)
{
}

void Camera::Update(void)
{
}

void Camera::Always(void)
{
}
