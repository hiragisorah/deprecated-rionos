#pragma once

#include "..\seed-engine\scene.h"

#include "displacement-field.h"

class Field : public Seed::Entity
{
public:
	Field(std::shared_ptr<Entity> & self)
		: Seed::Entity(self)
	{}

private:
	void Initialize(void) override
	{
		this->AddComponent<DisplacementField>();
	}
	void Finalize(void) override
	{

	}
};