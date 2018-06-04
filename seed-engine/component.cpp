#include "component.h"

Seed::Component::Component(std::shared_ptr<Component>& self)
	: self_(self)
{
}

const std::shared_ptr<Seed::Component>& Seed::Component::self(void) const
{
	return this->self_;
}

const std::weak_ptr<Seed::Entity>& Seed::Component::entity(void)
{
	return this->entity_;
}

void Seed::Component::set_entity(const std::shared_ptr<Entity>& entity)
{
	this->entity_ = entity;
}

void Seed::Component::Destroy(void)
{
	this->Finalize();
	this->self_.reset();
}
