#include "rionos.h"

#include <memory>

#include "test-scene.h"

void Rionos::Initialize(void)
{
	this->main_scene_ = std::make_shared<TestScene>(this->main_scene_);
	this->main_scene_->set_graphics(this->graphics());
	this->main_scene_->Initialize();
}

bool Rionos::Run(void)
{
	if (this->main_scene_->IsRunning())
	{
		this->main_scene_->Run();
	}
	else
	{
		this->Finalize();
		if (this->main_scene_->next_scene())
		{
			this->main_scene_ = this->main_scene_->next_scene();
			this->main_scene_->Initialize();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Rionos::Finalize(void)
{
}

void Rionos::Destroy(void)
{
	delete this;
}

Game * const Create(Graphics * const graphics)
{
	return new Rionos(graphics);
}