#pragma once

#include "utils/Utils.h"

namespace UserLogic
{
	namespace Components
	{
		// -v-v-v- Components -v-v-v-

		struct Test0
		{
			bool state = false;
		};

		struct Test1
		{
			int v = 666;
		};

		using ComponentsTypes = Utils::TypesPack<Test0, Test1>;

		// -^-^-^- Components -^-^-^-


		// -v-v-v- Bundles -v-v-v-


		// -^-^-^- Bundles -^-^-^-
	}
}