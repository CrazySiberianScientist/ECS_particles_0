#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace EngineLogic
{
	class AppSystem : public BaseSystem
	{
	public:
		AppSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		/*void init(SystemsOrders::Init::APP)
		{
			printf("!! %s %s\n", __FUNCTION__, "APP");
		}*/
	};
}