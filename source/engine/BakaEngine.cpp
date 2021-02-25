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
}