#pragma once

#include "utils/linmath.h"
#include "ECS.h"

namespace Baka
{
	namespace EngineComponents
	{
		// -v-v-v- Components -v-v-v-

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

		// -^-^-^- Components -^-^-^-


		// -v-v-v- Bundles -v-v-v-

		ECS_COMPONENT_BUNDLE(CameraBundle, Transform, CameraTransform);

		// -^-^-^- Bundles -^-^-^-
	}
}