#include "system.h"

Seed::System::System(std::shared_ptr<System>& self)
	: self_(self)
{
}

const std::shared_ptr<Seed::System>& Seed::System::self(void) const
{
	return this->self_;
}

const std::weak_ptr<Seed::Scene>& Seed::System::scene(void)
{
	return this->scene_;
}

void Seed::System::set_scene(const std::shared_ptr<Seed::Scene>& scene)
{
	this->scene_ = scene;
}

void Seed::System::Destroy(void)
{
	this->self_.reset();
}
