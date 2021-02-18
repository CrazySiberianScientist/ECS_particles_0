#include "BakaEngine.h"

#include <stdio.h>
#include <glad/gl.h>
#include <gl/GL.h>
#include "utils/linmath.h"

#include "Systems.h"

namespace Common
{
	Engine::Engine() 
	{
		construct_systems();
	}

	Engine::~Engine()
	{
		destruct_systems();
	}

	void Engine::run()
	{
		run_systems_preinits(SystemsTypes{});
		run_inits_orders(SystemsOrders::Init{});
	}

	template <typename ..._Systems>
	void Engine::run_systems_preinits(Utils::TypesPack<_Systems...>){ (run_system_preinit<_Systems>(), ...); }
	template<typename _SystemPtr>
	void Engine::run_system_preinit()
	{
		auto &system_info = std::get<SystemInfo<_SystemPtr>>(systems_info);
		if constexpr (SystemInfo<_SystemPtr>::init_methods_count == 0)
		{
			system_info.entities.insert(system_info.entities.end()
					, system_info.not_inited_entities.begin()
					, system_info.not_inited_entities.end());
		}
		else
		{
			system_info.passed_inits = 0;
			system_info.initing_entities.insert(system_info.initing_entities.end()
					, system_info.not_inited_entities.begin()
					, system_info.not_inited_entities.end());
		}
		system_info.not_inited_entities.clear();
	}

	template<typename ..._Orders>
	void Engine::run_inits_orders(Utils::TypesPack<_Orders...>) { (run_systems_inits<_Orders>(SystemsTypes{}), ...); }
	template <typename _Order, typename ..._Systems>
	void Engine::run_systems_inits(Utils::TypesPack<_Systems...>) { (run_system_init<_Systems, _Order>(), ...); }
	template<typename _SystemPtr, typename _Order>
	void Engine::run_system_init()
	{ 
		if constexpr (has_init<std::remove_pointer_t<_SystemPtr>, _Order>::value)
		{
			auto &system_info = std::get<SystemInfo<_SystemPtr>>(systems_info);

			for (const auto entity_id : system_info.initing_entities)
				std::get<_SystemPtr>(systems)->init(_Order{}, entity_id);
			++system_info.passed_inits;

			if (SystemInfo<_SystemPtr>::init_methods_count == system_info.passed_inits)
			{
				system_info.entities.insert(system_info.entities.end()
					, system_info.initing_entities.begin()
					, system_info.initing_entities.end());
				system_info.initing_entities.clear();
			}
		}
	}

	void Engine::construct_systems() { construct_systems_impl(SystemsCollection{}); }
	template<typename ..._Systems>
	void Engine::construct_systems_impl(std::tuple<_Systems...>) { (construct_system<_Systems>(), ...); }
	template<typename _SystemPtr>
	void Engine::construct_system() { std::get<_SystemPtr>(systems) = new std::remove_pointer_t<_SystemPtr>(*this); }

	void Engine::destruct_systems() { destruct_systems_impl(SystemsCollection{}); }
	template<typename ..._Systems> 
	void Engine::destruct_systems_impl(std::tuple<_Systems...>) { (destruct_system<_Systems>(), ...); }
	template<typename _SystemPtr>
	void Engine::destruct_system() { delete std::get<_SystemPtr>(systems); }
}


