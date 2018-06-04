#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "component.h"

namespace Seed
{
	class Entity
	{
		friend class Scene;

	public:
		Entity(std::shared_ptr<Entity> & self);
		virtual ~Entity(void);

		// 自分
	private:
		std::shared_ptr<Entity> & self_;

	public:
		void Destroy(void);

	public:
		virtual void Initialize(void) = 0;
		virtual void Finalize(void) = 0;

		// 親子関係
	private:
		std::weak_ptr<Scene> scene_;
		std::weak_ptr<Entity> parent_;
		std::vector<std::shared_ptr<Entity>> child_list_;

	public:
		template<class _Entity, class ... Args> const std::weak_ptr<_Entity> AddChild(const Args &... args)
		{
			this->child_list_.emplace_back(nullptr);
			auto & child_place = this->child_list_.back();
			auto child = std::make_shared<_Entity>(child_place, args ...);
			this->child_list_.back() = child;
			child_place->set_parent(this->self_);
			return child;
		}
		const std::weak_ptr<Entity> & parent(void);
		void set_parent(const std::shared_ptr<Entity> & parent);
		const std::weak_ptr<Scene> & scene(void);
		void set_scene(const std::shared_ptr<Scene> & scene);

	private:
		void PauseChildList(void);
		void UpdateChildList(void);
		void AlwaysChildList(void);

		// コンポーネント
	private:
		std::unordered_map<std::string, std::shared_ptr<Component>> component_list_;

	public:
		template<class _Component, class ... Args> void AddComponent(const Args &... args)
		{
			auto component_name = typeid(_Component).name();
			auto & component = this->component_list_[component_name];
			this->component_list_[component_name] = std::make_shared<_Component>(component, args ...);
			component->set_entity(this->self_);
			component->Initialize();
		}
		template<class _Component> const std::weak_ptr<_Component> GetComponent(void)
		{
			auto component_name = typeid(_Component).name();
			auto component = this->component_list_[component_name];
			return std::static_pointer_cast<_Component>(component);
		}
		template<class _Component> void RemoveComponent(void)
		{
			auto component_name = typeid(_Component).name()
			this->component_list_.erase(component_name);
		}

	private:
		void PauseComponentList(void);
		void UpdateComponentList(void);
		void AlwaysComponentList(void);
	};
}