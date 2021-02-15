#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace EngineLogic
{
	class AppSystem : public GenericSystem
	{
	public:
		AppSystem(Common::Engine &engine_) : GenericSystem(engine_) {}

		/*void init(SystemsOrders::Init::APP)
		{
			printf("!! %s %s\n", __FUNCTION__, "APP");
		}*/
	};
}