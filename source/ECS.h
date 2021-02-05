#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <array>

#include "utils/Utils.h"

namespace ECS
{
	using EntityIdType = uint32_t;
	static constexpr auto EntityIdType_Invalid = std::numeric_limits<EntityIdType>::max();

	template <typename ..._Components>
	using ComponentBundle = Utils::TypesPack<_Components...>;
	#define ECS_COMPONENT_BUNDLE(NAME, ...)\
	using NAME = ECS::ComponentBundle<__VA_ARGS__>;\
	constexpr NAME NAME##_v {};


	class EntityManager
	{
	public:
		EntityIdType create()
		{
			if (remained_entities.size())
			{
				const auto e = remained_entities.back();
				remained_entities.pop_back();
				entities.emplace_back(e);
				return e;
			}
			if (last_id >= EntityIdType_Invalid)
				return EntityIdType_Invalid;
			entities.emplace_back(last_id);
			return last_id++;
		}

		bool remove(const EntityIdType entity)
		{
			if (entity == EntityIdType_Invalid) return false;

			auto found_it = std::find(entities.begin(), entities.end(), entity);
			if (found_it == entities.end()) return false;

			remained_entities.emplace_back(*found_it);
			Utils::removeFast(found_it, entities);
			return true;
		}

		const auto &getEntities() const { return entities; }

	private:
		std::vector<EntityIdType> entities;
		std::vector<EntityIdType> remained_entities;
		EntityIdType last_id{};
	};

	// TODO: few components per entity
	template<typename _ComponentType>
	class ComponentCollection
	{
	public:
		_ComponentType* get(const EntityIdType entity)
		{
			if (auto c = entity_to_component.get(entity))
				return *c;
			return nullptr;
		}

		template<class ...Args>
		_ComponentType* emplace(const EntityIdType entity, Args &&...args)
		{
			if (entity == EntityIdType_Invalid)
				return nullptr;
			if (auto c = get(entity))
				return c;

			auto new_component = components.emplace_back(std::forward<Args>(args)...);
			entity_to_component.emplace(entity, new_component);

			return new_component;
		}

		_ComponentType* create(const EntityIdType entity, _ComponentType && component)
		{
			return emplace(entity, std::move(component));
		}

		_ComponentType* create(const EntityIdType entity, const _ComponentType & component)
		{
			return emplace(entity, component);
		}

		void remove(const EntityIdType entity)
		{
			if (entity == EntityIdType_Invalid) return;
			auto component = get(entity);
			if (!component) return;

			entity_to_component.remove(entity);
			components.remove(component);
		}

	private:
		Utils::ChunkBuffer<_ComponentType> components;
		Utils::ChunkTable<_ComponentType*> entity_to_component;
	};

	template<typename ..._ComponentTypes>
	class ComponentManager
	{
	public:
		template<typename _Component>
		_Component* getComponent(const EntityIdType entity)
		{
			return get_collection<_Component>().get(entity);
		}

		template<typename _Component, class ...Args>
		_Component* emplaceComponent(const EntityIdType entity, Args &&...args)
		{
			return get_collection<_Component>().emplace(entity, std::forward<Args>(args)...);
		}

		template<typename _Component>
		_Component* createComponent(const EntityIdType entity, _Component && component)
		{
			return get_collection<_Component>().create(entity, std::move(component));
		}

		template<typename _Component>
		_Component* createComponent(const EntityIdType entity, const _Component & component = {})
		{
			return get_collection<_Component>().create(entity, component);
		}

		template<typename ..._Components>
		void createBundle(ComponentBundle<_Components...>, const EntityIdType entity, _Components ...components)
		{
			(emplaceComponent<_Components>(entity, std::move(components)), ...);
		}

		template<typename ..._Components>
		void createBundle(ComponentBundle<_Components...>, const EntityIdType entity)
		{
			(emplaceComponent<_Components>(entity), ...);
		}

		template<typename _Component>
		void removeComponent(const EntityIdType entity)
		{
			get_collection<_Component>().remove(entity);
		}
		
		void removeAllComponents(const EntityIdType entity)
		{
			(get_collection<_ComponentTypes>().remove(entity), ...);
		}

	private:
		template<typename _Component>
		ComponentCollection<_Component>& get_collection()
		{
			return std::get<ComponentCollection<_Component>>(component_collections);
		}

	private:
		std::tuple<ComponentCollection<_ComponentTypes>...> component_collections;
	};
}