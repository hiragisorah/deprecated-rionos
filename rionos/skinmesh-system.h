#pragma once

#include <DirectXMath.h>

#include "..\seed-engine\system.h"

class SkinMeshSystem : public Seed::System
{
public:
	SkinMeshSystem(std::shared_ptr<System> & self);

public:
	void Initalize(void) override;
	void Finalize(void) override;
	void Pause(void) override;
	void Update(void) override;
	void Always(void) override;
};