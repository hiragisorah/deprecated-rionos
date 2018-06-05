#include "scene.h"

Seed::Scene::Scene(std::shared_ptr<Scene>& self)
	: self_(self)
	, running_(true)
	, pause_(false)
{
}

Seed::Scene::~Scene(void)
{
}

void Seed::Scene::Shutdown(void)
{
	this->running_ = false;
}

void Seed::Scene::Pause(void)
{
	this->pause_ = true;
}

void Seed::Scene::Resume(void)
{
	this->pause_ = false;
}

const bool & Seed::Scene::IsPause(void)
{
	return this->pause_;
}

const bool & Seed::Scene::IsRunning(void)
{
	return this->running_;
}

std::shared_ptr<Seed::Scene>& Seed::Scene::next_scene(void)
{
	return this->next_scene_;
}

Graphics * const Seed::Scene::graphics(void)
{
	return this->graphics_;
}

void Seed::Scene::set_graphics(Graphics * const graphics)
{
	this->graphics_ = graphics;
}

void Seed::Scene::PauseChildList(void)
{
	this->PauseSystemList();

	for (unsigned int n = 0; n < this->child_list_.size(); ++n)
	{
		auto & child = this->child_list_[n];

		if (child->self_)
		{
			child->PauseChildList();
		}
		else
		{
			this->child_list_.erase(this->child_list_.begin() + n);
		}
	}
}

void Seed::Scene::UpdateChildList(void)
{
	this->UpdateSystemList();

	for (unsigned int n = 0; n < this->child_list_.size(); ++n)
	{
		auto & child = this->child_list_[n];

		if (child->self_)
		{
			child->UpdateChildList();
		}
		else
		{
			this->child_list_.erase(this->child_list_.begin() + n);
		}
	}
}

void Seed::Scene::AlwaysChildList(void)
{
	this->AlwaysSystemList();

	for (unsigned int n = 0; n < this->child_list_.size(); ++n)
	{
		auto & child = this->child_list_[n];

		if (child->self_)
		{
			child->AlwaysChildList();
		}
		else
		{
			this->child_list_.erase(this->child_list_.begin() + n);
		}
	}
}

void Seed::Scene::PauseSystemList(void)
{
	for (auto & system_pair : this->system_list_)
	{
		auto & name = system_pair.first;
		auto & system = system_pair.second;

		if (system->self())
		{
			system->Pause();
		}
		else
		{
			this->system_list_.erase(name);
		}
	}
}

void Seed::Scene::UpdateSystemList(void)
{
	for (auto & system_pair : this->system_list_)
	{
		auto & name = system_pair.first;
		auto & system = system_pair.second;

		if (system->self())
		{
			system->Update();
		}
		else
		{
			this->system_list_.erase(name);
		}
	}
}

void Seed::Scene::AlwaysSystemList(void)
{
	for (auto & system_pair : this->system_list_)
	{
		auto & name = system_pair.first;
		auto & system = system_pair.second;

		if (system->self())
		{
			system->Always();
		}
		else
		{
			this->system_list_.erase(name);
		}
	}
}
