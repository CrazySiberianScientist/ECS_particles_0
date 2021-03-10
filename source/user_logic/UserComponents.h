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

		using ComponentsTypes = Utils::TypesPack<Speed>;

		// -^-^-^- Components -^-^-^-


		// -v-v-v- Bundles -v-v-v-


		// -^-^-^- Bundles -^-^-^-
	}
}