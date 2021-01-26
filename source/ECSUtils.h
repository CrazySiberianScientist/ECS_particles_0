#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>

namespace ECSUtils
{
	template<size_t _MAX_VALUE>
	class PreferredIntegralType
	{
		using Types = std::tuple<uint8_t, uint16_t, uint32_t, uint64_t>;
		
		template<size_t _V>
		using PristineType = typename std::decay<decltype(std::get<_V>(Types()))>::type;

		template<size_t _INDEX>
		struct check_limit
		{
			static constexpr auto value =
				_MAX_VALUE >= std::numeric_limits<PristineType<_INDEX - 1>>::max()
				&& _MAX_VALUE <= std::numeric_limits<PristineType<_INDEX>>::max();
		};

		static constexpr size_t value_to_index()
		{
			static_assert(_MAX_VALUE >= 0, "Must be >= 0");
			if constexpr (_MAX_VALUE <= std::numeric_limits<PristineType<0>>::max()) return 0;
			else if constexpr (check_limit<1>::value) return 1;
			else if constexpr (check_limit<2>::value) return 2;
			else if constexpr (check_limit<3>::value) return 3;
		}

	public:
		using type = PristineType<value_to_index()>;
	};

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


		public:
			std::vector<_ComponentType> components;
			std::array<EntityType, _ENTITIES_MAX_NUM> entity_to_component;

		};

	};
}