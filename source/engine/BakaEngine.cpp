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


