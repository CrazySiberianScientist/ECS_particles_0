#pragma once

#include "utils/linmath.h"

namespace Baka
{
	#define ENGINE_COMPONENTS_TYPES Transform, Camera
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