#pragma once

#include "utils/linmath.h"

namespace Baka
{
	#define ENGINE_COMPONENT(COMPONENT) EngineComponents::COMPONENT
	#define ENGINE_COMPONENTS\
	ENGINE_COMPONENT(Transform),\
	ENGINE_COMPONENT(Camera)

	namespace EngineComponents
	{
		struct Transform
		{
			vec3 pos;
			quat rot;
			vec3 scale;
		};

		struct Camera
		{

		};
	}
}