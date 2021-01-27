#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <array>

namespace ECSUtils
{
	template<size_t _ENTITIES_MAX_NUM, size_t _COMPONENTS_PER_ENTITY_AVERAGE>
	class ECS
	{
	public:
		using EntityType = typename PreferredIntegralType<_ENTITIES_MAX_NUM>::type;
		using ComponentIndexType = typename PreferredIntegralType<_ENTITIES_MAX_NUM * _COMPONENTS_PER_ENTITY_AVERAGE>::type;

		class EntityManager
		{
		public:
			std::tuple<EntityType, bool> create()
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
			std::vector<EntityType> entities;
			std::vector<EntityType> remained_entities;
			EntityType last_id = 0;
		};

		template<typename _ComponentType>
		class ComponentManager
		{
		public:
			_ComponentType &create(const EntityType entity)
			{

			}

		private:
			std::vector<_ComponentType> components;
			std::array<EntityType, _ENTITIES_MAX_NUM> entity_to_component;
			std::array<_ComponentType, _ENTITIES_MAX_NUM> entity_to_component;

		};

	};
}