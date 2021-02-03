#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <array>

#include "utils/Utils.h"

namespace ECSUtils
{
	using EntityIdType = uint32_t;
	static constexpr auto EntityIdType_Invalid = std::numeric_limits<EntityIdType>::max();

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

	private:
		std::vector<EntityIdType> entities;
		std::vector<EntityIdType> remained_entities;
		EntityIdType last_id{};
	};

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
		_ComponentType* create(const EntityIdType entity, Args &&...args)
		{
			if (entity == EntityIdType_Invalid)
				return nullptr;
			if (auto c = get(entity))
				return c;

			auto new_component = components.emplace_back(std::forward<Args>(args)...);
			entity_to_component.emplace(entity, new_component);

			return new_component;
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
		_Component* createComponent(const EntityIdType entity, Args &&...args)
		{
			if constexpr (std::is_constructible_v<_Component, Args...>)
				return get_collection<_Component>().create(entity, std::forward<Args>(args)...);
			return get_collection<_Component>().create(entity, _Component{ std::forward<Args>(args)... });
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