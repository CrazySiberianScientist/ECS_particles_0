#include "BakaEngine.h"

#include <stdio.h>
#include <glad/gl.h>
#include <gl/GL.h>
#include "utils/linmath.h"

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

		if (is_needed_to_stop)
		{
			// destroy
			return;
		}
	}

	/* TODO: I thought about realization via exception, but I'm not sure.
	There is needed way to immediately engine stop.*/
	void Engine::stop()
	{
		is_needed_to_stop = true;
	}

}