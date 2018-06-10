#pragma once

#include <iostream>

#include "..\seed-engine\scene.h"

#include "camera.h"
#include "player.h"
#include "field.h"

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
		this->graphics()->LoadShader(ShaderPath::tesselation);
		this->graphics()->LoadShader(ShaderPath::shader_3d);
		this->graphics()->LoadTexture(TexturePath::taisetsu);
		this->graphics()->LoadTexture(TexturePath::Simple);
		this->graphics()->LoadTexture(TexturePath::Simple_normal);
		this->graphics()->LoadModel(ModelPath::kaoru);

		this->AddSystem<Camera>();

		this->AddChild<Player>();
		this->AddChild<Field>();
	}
	void Finalize(void) override {}
};