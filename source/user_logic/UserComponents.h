#pragma once

#include "utils/Utils.h"

namespace UserLogic
{
	namespace Components
	{
		// -v-v-v- Components -v-v-v-

		struct Speed
		{
			glm::vec3 move_vec;
		};

		struct ShaderObject
		{
			std::string fragment_shader_name;
			std::string vertex_shader_name;
		};

		using ComponentsTypes = Utils::TypesPack<Speed, ShaderObject>;

		// -^-^-^- Components -^-^-^-


		// -v-v-v- Bundles -v-v-v-


		// -^-^-^- Bundles -^-^-^-
	}
}