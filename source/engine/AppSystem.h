#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace EngineLogic
{
	class AppSystem : public BaseSystem
	{
	public:
		void init(SystemsOrders::Init::APP)
		{
			printf("!! %s %s\n", __FUNCTION__, "APP");
		}
	};
}