#pragma once

#include "utils/linmath.h"
#include "ECS.h"

namespace Baka
{
	namespace EngineComponents
	{
		#define ENGINE_COMPONENT(COMPONENT) EngineComponents::COMPONENT
		#define ENGINE_COMPONENTS\
		ENGINE_COMPONENT(Transform),\
		ENGINE_COMPONENT(CameraTransform)


		struct Transform
		{
			vec3 pos;
			quat rot;
			vec3 scale;
		};

		struct CameraTransform
		{
			mat4x4 modelview;
			mat4x4 projection;
		};

		using ComponentsTypes = Utils::TypesPack<Transform, CameraTransform>;
		ECS_COMPONENT_BUNDLE(CameraBundle, Transform, CameraTransform);
	}
}