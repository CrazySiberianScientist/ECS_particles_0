#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace EngineLogic
{
	class CameraSystem : public BaseSystem
	{
	public:
		CameraSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void init(SystemsOrders::Init::RENDER)
		{
			printf("!! %s %s\n", __FUNCTION__, "RENDER");
		}
	};
}