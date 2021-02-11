#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace Baka
{
	class AppSystem : public BaseSystem
	{
	public:
		void init(EngineSystemsOrders::Init::APP)
		{
			printf("!! %s %s\n", __FUNCTION__, "APP");
		}
	};
}