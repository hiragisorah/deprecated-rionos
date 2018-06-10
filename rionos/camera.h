#pragma once

#include <DirectXMath.h>

#include "..\seed-engine\system.h"

class Camera : public Seed::System
{
public:
	Camera(std::shared_ptr<System> & self);

private:
	DirectX::XMVECTOR eye_;
	DirectX::XMVECTOR at_;
	DirectX::XMVECTOR up_;

	float fov_;
	float aspect_;
	float near_;
	float far_;

public:
	DirectX::XMMATRIX view(void);
	DirectX::XMMATRIX projection(void);

public:
	void Initalize(void) override;
	void Finalize(void) override;
	void Pause(void) override;
	void Update(void) override;
	void Always(void) override;
};