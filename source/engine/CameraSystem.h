#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace EngineLogic
{
	class CameraSystem : public BaseSystem
	{
	public:
		void init(SystemsOrders::Init::RENDER)
		{
			printf("!! %s %s\n", __FUNCTION__, "RENDER");
		}
	};
}