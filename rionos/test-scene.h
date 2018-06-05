#pragma once

#include <iostream>

#include "..\seed-engine\scene.h"

#include "test-entity.h"

class TestScene : public Seed::Scene
{
public:
	TestScene(std::shared_ptr<Scene> & self)
		: Seed::Scene(self)
	{

	}

private:
	void Initialize(void) override
	{
		this->graphics()->LoadShader(Resource::Shader::PATH::shader_3d);
		this->graphics()->LoadModel(Resource::Model::PATH::jeep);

		this->AddChild<TestEntity>();
	}
	void Finalize(void) override {}
};