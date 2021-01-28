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
		using ComponentIndexType = typename PreferredIntegralType<_ENTITIES_MAX_NUM * _COMPONENTS_PER_ENTITY_AVERAGE>::type;

		class EntityManager
		{
		public:
			std::tuple<EntityIdType, bool> create()
			{
				if (remained_entities.size())
				{
					const auto e = remained_entities.back();
					remained_entities.pop_back();
					entities.emplace_back(e);
					return { e, true };
				}
				if (last_id >= _ENTITIES_MAX_NUM)
					return { last_id, false };
				entities.emplace_back(last_id);
				return { last_id++, true };
			}

			bool remove(const EntityType entity)
			{
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
			_ComponentType &create(const EntityIdType entity)
			{
				auto component_index = ComponentIndexType_INVALID_VAL;
				if (entity >= entity_to_component.size())
				{
					entity_to_component.resize(entity + 1);
				}
				else if (entity_to_component[entity])
				{

				}

			}

		private:
			std::vector<_ComponentType> components;
			std::vector<ComponentIndexType> remained_components;
			std::vector<ComponentIndexType> entity_to_component;
			std::vector<EntityIdType> component_to_entity;

		};

	};
}