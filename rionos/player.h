#pragma once

#include "..\seed-engine\entity.h"

#include "field-collision.h"
#include "transform.h"

class Player : public Seed::Entity
{
public:
	Player(std::shared_ptr<Entity> & self)
		: Seed::Entity(self)
	{}

private:
	void Initialize(void) override
	{
		this->AddComponent<Transform>();
		this->AddComponent<FieldCollision>(TexturePath::Simple);
	}
	void Finalize(void) override
	{

	}
};