#pragma once

#include "engine/BaseSystem.h"
#include "UserSystemsOrders.h"

namespace UserLogic
{

	class TestLogicSystem : public GenericSystem
	{
	public:
		TestLogicSystem(Common::Engine &engine_) : GenericSystem(engine_) {}

		void init(SystemsOrders::Init::TEST_0, const ECS::EntityIdType);

		void init(SystemsOrders::Init::SCENE_0, const ECS::EntityIdType)
		{
			printf("!! %s %s\n", __FUNCTION__, "SCENE_0");
		}

		void init(SystemsOrders::Init::TEST_1, const ECS::EntityIdType)
		{
			printf("!! %s %s\n", __FUNCTION__, "TEST_1");
		}
	};

}