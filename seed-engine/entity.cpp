#include "entity.h"

Seed::Entity::Entity(std::shared_ptr<Entity>& self)
	: self_(self)
{
}

Seed::Entity::~Entity(void)
{
}

void Seed::Entity::Destroy(void)
{
	this->Finalize();
	this->self_.reset();
}

void Seed::Entity::set_parent(const std::shared_ptr<Seed::Entity>& parent)
{
}

const std::weak_ptr<Seed::Scene>& Seed::Entity::scene(void)
{
	return this->scene_;
}

void Seed::Entity::set_scene(const std::shared_ptr<Seed::Scene>& scene)
{
	this->scene_ = scene;
}

const std::weak_ptr<Seed::Entity>& Seed::Entity::parent(void)
{
	return this->parent_;
}

void Seed::Entity::PauseChildList(void)
{
	this->PauseComponentList();

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

void Seed::Entity::UpdateChildList(void)
{
	this->UpdateComponentList();

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

void Seed::Entity::AlwaysChildList(void)
{
	this->AlwaysComponentList();

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

void Seed::Entity::PauseComponentList(void)
{
	for (auto & component_pair : this->component_list_)
	{
		auto & name = component_pair.first;
		auto & component = component_pair.second;

		if (component->self())
		{
			component->Pause();
		}
		else
		{
			this->component_list_.erase(name);
		}
	}
}

void Seed::Entity::UpdateComponentList(void)
{
	for (auto & component_pair : this->component_list_)
	{
		auto & name = component_pair.first;
		auto & component = component_pair.second;

		if (component->self())
		{
			component->Update();
		}
		else
		{
			this->component_list_.erase(name);
		}
	}
}

void Seed::Entity::AlwaysComponentList(void)
{
	for (auto & component_pair : this->component_list_)
	{
		auto & name = component_pair.first;
		auto & component = component_pair.second;

		if (component->self())
		{
			component->Always();
		}
		else
		{
			this->component_list_.erase(name);
		}
	}
}
