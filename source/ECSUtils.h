#pragma once

#include <vector>
#include <tuple>
#include <limits>

namespace ECSUtils
{
	template<size_t _MAX_VALUE>
	struct PreferredIntegralType
	{
		using Types = std::tuple<uint8_t, uint16_t, uint32_t, uint64_t>;
		
		template<size_t _V>
		using PristineType = std::decay<decltype(std::get<_V>(Types()))>::type;

		template<size_t _INDEX>
		struct check_limit
		{
			static constexpr auto value =
				_MAX_VALUE >= std::numeric_limits<PristineType<_INDEX - 1>>::max()
				&& _MAX_VALUE <= std::numeric_limits<PristineType<_INDEX>>::max();
		};

		auto value_to_index() constexpr
		{

			static_assert(MAX_VALUE < 0, "Must be >= 0");
			if constexpr (MAX_VALUE >= 0 && MAX_VALUE <= std::numeric_limits<PristineType<0>>::max()) return 0;
			//if constexpr () return 0;
		}
	};

	template<size_t ENTITIES_MAX_NUM>
	class ECS
	{
		

		template<typename _EntityType = int>
		class EntityManager
		{
			std::vector<_EntityType> entities;
		};

		template<typename _ComponentType>
		class ComponentManager
		{



			std::vector<_ComponentType> components;

		};

	};
}