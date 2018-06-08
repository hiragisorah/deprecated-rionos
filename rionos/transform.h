#pragma once

#include <DirectXMath.h>
#include "..\seed-engine\component.h"

class Transform : public Seed::Component
{
public:
	Transform(std::shared_ptr<Component> & self)
		: Seed::Component(self)
		, position_(DirectX::XMFLOAT3(0,0,0))
		, rotation_(DirectX::XMFLOAT3(0, 0, 0))
		, scale_(DirectX::XMFLOAT3(1, 1, 1))
	{}

private:
	DirectX::XMFLOAT3 position_;
	DirectX::XMFLOAT3 rotation_;
	DirectX::XMFLOAT3 scale_;

public:
	const DirectX::XMFLOAT3 & position(void) const;
	const DirectX::XMFLOAT3 & rotation(void) const;
	const DirectX::XMFLOAT3 & scale(void) const;

public:
	void set_position(DirectX::XMFLOAT3 position);
	void set_rotation(DirectX::XMFLOAT3 rotation);
	void set_scale(DirectX::XMFLOAT3 scale);

public:
	DirectX::XMMATRIX GetTransformMatrix(void);

private:
	void Initialize(void) override {}
	void Finalize(void) override {}
	void Pause(void) override {}
	void Update(void) override {}
	void Always(void) override {}
};