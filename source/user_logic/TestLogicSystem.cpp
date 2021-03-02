#include "TestLogicSystem.h"
#include "engine/Engine.h"

namespace UserLogic
{
	void TestLogicSystem::init(SystemsOrders::Init::TEST_0, const ECS::EntityIdType)
	{
		printf("!! %s %s\n", __FUNCTION__, "TEST_0");
	}

	void TestLogicSystem::update(SystemsOrders::Update::TEST_1, const ECS::EntityIdType e)
	{
		printf("!! %s %s %d\n", __FUNCTION__, "TEST_1", e);

		static int loh = 0;
		if (loh++ >= 10)
			engine.stop();
	}

}
