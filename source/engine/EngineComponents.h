#pragma once

#include "utils/linmath.h"
#include "ecs/ECS.h"

namespace EngineLogic
{
	namespace Components
	{
		// -v-v-v- Components -v-v-v-

		struct Transform
		{
			vec3 pos{};
			quat rot{};
			vec3 scale{1.0f, 1.0f, 1.0f};
		};

		struct CameraTransform
		{
			CameraTransform() { mat4x4_identity(modelview); mat4x4_identity(projection); }

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