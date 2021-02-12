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

	}

	void Engine::run()
	{
		run_inits_orders(SystemsOrders::Init{});
	}
}