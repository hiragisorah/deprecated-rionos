#pragma once

#include "..\seed-engine\entity.h"

#include "field-collision.h"
#include "character-model.h"
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
		this->AddComponent<CharacterModel>(ModelPath::kaoru);

		this->GetComponent<Transform>().lock()->set_scale(DirectX::XMFLOAT3(0.02f, 0.02f, 0.02f));
	}
	void Finalize(void) override
	{

	}
};