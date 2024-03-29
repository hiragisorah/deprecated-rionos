#pragma once

#include <memory>

namespace Seed
{
	class Entity;

	class Component
	{
	public:
		Component(std::shared_ptr<Component> & self);
		
		// 自分
	private:
		std::shared_ptr<Component> & self_;

	public:
		const std::shared_ptr<Component> & self(void) const;

		// 親
	private:
		std::weak_ptr<Entity> entity_;

	public:
		const std::weak_ptr<Entity> & entity(void);
		void set_entity(const std::shared_ptr<Entity> & entity);

		// 仕事
	public:
		virtual void Initialize(void) = 0;
		virtual void Finalize(void) = 0;
		virtual void Pause(void) = 0;
		virtual void Update(void) = 0;
		virtual void Always(void) = 0;

	public:
		void Destroy(void);
	};
}