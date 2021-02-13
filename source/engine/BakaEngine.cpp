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
		run_inits_orders(SystemsOrders::Init{});
	}


	template<typename ..._Orders>
	void Engine::run_inits_orders(Utils::TypesPack<_Orders...>) { (run_systems_inits<_Orders>(SystemsCollection{}), ...); }
	template <typename _Order, typename ..._Systems>
	void Engine::run_systems_inits(std::tuple<_Systems...>) { (run_system_init<_Systems, _Order>(), ...); }
	template<typename _SystemPtr, typename _Order>
	void Engine::run_system_init() { if constexpr (has_init<std::remove_pointer_t<_SystemPtr>, _Order>::value) std::get<_SystemPtr>(systems)->init(_Order{}); }

	void Engine::construct_systems() { construct_systems_impl(SystemsCollection{}); }
	template<typename ..._Systems>
	void Engine::construct_systems_impl(std::tuple<_Systems...>) { (construct_system<_Systems>(), ...); }
	template<typename _SystemPtr>
	void Engine::construct_system() { std::get<_SystemPtr>(systems) = new std::remove_pointer_t<_SystemPtr>; }

	void Engine::destruct_systems() { destruct_systems_impl(SystemsCollection{}); }
	template<typename ..._Systems> 
	void Engine::destruct_systems_impl(std::tuple<_Systems...>) { (destruct_system<_Systems>(), ...); }
	template<typename _SystemPtr>
	void Engine::destruct_system() { delete std::get<_SystemPtr>(systems); }
}


