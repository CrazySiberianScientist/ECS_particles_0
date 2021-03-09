#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ecs/ECS.h"

namespace EngineLogic
{
	namespace Components
	{
		// -v-v-v- Components -v-v-v-

		struct Transform
		{
			glm::vec3 pos{};
			glm::quat rot{};
			glm::vec3 scale{1.0f, 1.0f, 1.0f};
		};

		struct CameraTransform
		{
			glm::mat4 view{};
			glm::mat4 projection{};
			// TODO: free camera mode
			glm::vec3 target_pos{};
			float fov_y = 60.0f;
			float z_near = 0.0f;
			float z_far = 100.0f;
		};

		using ComponentsTypes = Utils::TypesPack<Transform, CameraTransform>;

		// -^-^-^- Components -^-^-^-


		// -v-v-v- Bundles -v-v-v-

		ECS_COMPONENT_BUNDLE(CameraBundle, Transform, CameraTransform);

		// -^-^-^- Bundles -^-^-^-
	}
}