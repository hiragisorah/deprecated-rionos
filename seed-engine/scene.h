#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "..\graphics.h"

#include "entity.h"
#include "system.h"

namespace Seed
{
	class Scene
	{
	public:
		Scene(std::shared_ptr<Scene> & self);
		virtual ~Scene(void);

		// 自分
	private:
		std::shared_ptr<Scene> & self_;
		bool running_;
		bool pause_;

	public:
		void Shutdown(void);
		void Pause(void);
		void Resume(void);
		const bool & IsPause(void);
		const bool & IsRunning(void);

	public:
		virtual void Initialize(void) = 0;
		virtual void Finalize(void) = 0;

	public:
		void Run(void)
		{
			if (this->pause_)
			{
				this->PauseSystemList();
				this->PauseChildList();
			}
			else
			{
				this->UpdateSystemList();
				this->UpdateChildList();
			}

			this->AlwaysSystemList();
			this->AlwaysChildList();
		}

		// グラフィックス
	private:
		Graphics * graphics_;

	public:
		Graphics * const graphics(void);
		void set_graphics(Graphics * const graphics);

		// 次シーン
	private:
		std::shared_ptr<Scene> next_scene_;

	public:
		std::shared_ptr<Scene> & next_scene(void);
		template<class _Scene, class ... Args> void set_next_scene(const Args &... args)
		{
			this->next_scene_ = std::make_shared<_Scene>(args ...);
		};
		template<class _Scene, class ... Args> void change_scene(const Args &... args)
		{
			this->set_next_scene<Scene>(args ...);
			this->Shutdown();
		}

		// 親子関係
	private:
		std::vector<std::shared_ptr<Entity>> child_list_;

	public:
		template<class _Entity, class ... Args> const std::weak_ptr<_Entity> AddChild(const Args &... args)
		{
			this->child_list_.emplace_back(nullptr);
			auto & child_place = this->child_list_.back();
			auto child = std::make_shared<_Entity>(child_place, args ...);
			this->child_list_.back() = child;
			child_place->set_scene(this->self_);
			child_place->Initialize();
			return child;
		}

	private:
		void PauseChildList(void);
		void UpdateChildList(void);
		void AlwaysChildList(void);

		// コンポーネント
	private:
		std::unordered_map<std::string, std::shared_ptr<System>> system_list_;

	public:
		template<class _System, class ... Args> void AddSystem(const Args &... args)
		{
			auto system_name = typeid(_System).name();
			auto & system = this->system_list_[system_name];
			this->system_list_[system_name] = std::make_shared<_System>(system, args ...);
			system->set_scene(this->self_);
			system->Initalize();
		}
		template<class _System> const std::weak_ptr<_System> GetSystem(void)
		{
			auto system_name = typeid(_System).name();
			auto system = this->system_list_[system_name];
			return std::static_pointer_cast<_System>(system);
		}
		template<class _System> void RemoveSystem(void)
		{
			auto system_name = typeid(_System).name()
				this->system_list_.erase(system_name);
		}

	private:
		void PauseSystemList(void);
		void UpdateSystemList(void);
		void AlwaysSystemList(void);
	};
}