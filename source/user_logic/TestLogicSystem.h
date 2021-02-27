#pragma once

#include "engine/BaseSystem.h"
#include "UserSystemsOrders.h"

namespace UserLogic
{

	class TestLogicSystem : public BaseSystem
	{
	public:
		TestLogicSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void init(SystemsOrders::Init::TEST_0, const ECS::EntityIdType);

		void init(SystemsOrders::Init::SCENE_0, const ECS::EntityIdType e)
		{
			printf("!! %s %s %d\n", __FUNCTION__, "SCENE_0", e);
		}

		void init(SystemsOrders::Init::TEST_1, const ECS::EntityIdType e)
		{
			printf("!! %s %s %d\n", __FUNCTION__, "TEST_1", e);
		}

		void destroy(SystemsOrders::Destroy::TEST_0, const ECS::EntityIdType e)
		{
			printf("!! %s %s %d\n", __FUNCTION__, "TEST_0", e);
		}
		
		void destroy(SystemsOrders::Destroy::TEST_1, const ECS::EntityIdType e)
		{
			printf("!! %s %s %d\n", __FUNCTION__, "TEST_1", e);
		}
 
	};

}