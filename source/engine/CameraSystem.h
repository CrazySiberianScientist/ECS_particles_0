#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace Baka
{
	class CameraSystem : public BaseSystem
	{
	public:
		void init(EngineSystemsOrders::Init::RENDER)
		{
			printf("!! %s %s\n", __FUNCTION__, "RENDER");
		}
	};
}