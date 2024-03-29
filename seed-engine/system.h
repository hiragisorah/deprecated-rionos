#pragma once

#include <memory>

namespace Seed
{
	class Scene;

	class System
	{
	public:
		System(std::shared_ptr<System> & self);

		// 自分
	private:
		std::shared_ptr<System> & self_;

	public:
		const std::shared_ptr<System> & self(void) const;

		// 親
	private:
		std::weak_ptr<Scene> scene_;

	public:
		const std::weak_ptr<Scene> & scene(void);
		void set_scene(const std::shared_ptr<Scene> & scene);

		// 仕事
	public:
		virtual void Initalize(void) = 0;
		virtual void Finalize(void) = 0;
		virtual void Pause(void) = 0;
		virtual void Update(void) = 0;
		virtual void Always(void) = 0;

	public:
		void Destroy(void);
	};
}