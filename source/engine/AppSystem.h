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

		void init(SystemsOrders::Init::APP, const ECS::EntityIdType e)
		{
			printf("!! %s %s %d\n", __FUNCTION__, "APP", e);
		}

		void destroy(SystemsOrders::Destroy::APP, const ECS::EntityIdType e)
		{
			printf("!! %s %s %d\n", __FUNCTION__, "APP", e);
		}
	};
}