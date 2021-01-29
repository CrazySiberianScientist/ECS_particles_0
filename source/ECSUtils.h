#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <array>

#include "utils/Utils.h"

namespace ECSUtils
{
	template<size_t _ENTITIES_MAX_NUM, size_t _COMPONENTS_PER_ENTITY_AVERAGE>
	class ECS
	{
	public:
		using EntityIdType = typename PreferredIntegralType<_ENTITIES_MAX_NUM>::type;
		static constexpr auto EntityIdType_Invalid = std::numeric_limits<EntityIdType>::max();
		using ComponentIndexType = typename PreferredIntegralType<_ENTITIES_MAX_NUM * _COMPONENTS_PER_ENTITY_AVERAGE>::type;
		static constexpr auto ComponentIndexType_Invalid = std::numeric_limits<ComponentIndexType>::max();

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
				if (last_id >= _ENTITIES_MAX_NUM)
					return EntityIdType_Invalid;
				entities.emplace_back(last_id);
				return last_id++;
			}

			bool remove(const EntityType entity)
			{
				if (entity == EntityIdType_Invalid) return false;

				auto found_it = std::find(entities.begin(), entities.end(), entity);
				if (found_it == entities.end()) return false;

				remained_entities.emplace_back(*found_it);
				*found_it = entities.back();
				entities.pop_back();
				return true;
			}

		private:
			std::vector<EntityIdType> entities;
			std::vector<EntityIdType> remained_entities;
			EntityIdType last_id = 0;
		};

		template<typename _ComponentType>
		class ComponentManager
		{
		public:
			_ComponentType *getComponentFor(const EntityIdType entity)
			{
				return components[entity_to_component[entity]];
			}

			_ComponentType *createFor(const EntityIdType entity)
			{
				if (entity == EntityIdType_Invalid) 
					return nullptr;

				auto component_index = entity_to_component[entity];
				if (component_index != ComponentIndexType_Invalid)
					return &components[component_index];
				if (remained_components.size())
				{
					component_index = remained_components.back();
					remained_components.pop_back();
				}
				else
				{
					component_index = components.size();
					components.push_back();
					component_to_entity.push_back();
				}

				if (entity >= entity_to_component.size())
					entity_to_component.resize(entity + 1, ComponentIndexType_Invalid);
				entity_to_component[entity] = component_index;
				component_to_entity[component_index] = entity;

				return &components[component_index];
			}

			bool removeFrom(const EntityIdType entity)
			{
				if (entity == EntityIdType_Invalid || entity >= entity_to_component.size()) 
					return false;

				auto &component_index = entity_to_component[entity];
				component_to_entity[component_index] = EntityIdType_Invalid;
				remained_components.emplace_back(component_index);
				component_index = ComponentIndexType_Invalid;
				
				return true;
			}

		private:
			//TODO: Chunk Vector
			std::vector<_ComponentType> components;

			std::vector<ComponentIndexType> remained_components;
			std::vector<ComponentIndexType> entity_to_component;
			std::vector<EntityIdType> component_to_entity;

		};

	};
}