#include "TestLogicSystem.h"
#include "engine/BakaEngine.h"

namespace UserLogic
{
	void TestLogicSystem::init(SystemsOrders::Init::TEST_0, const ECS::EntityIdType)
	{
		printf("!! %s %s\n", __FUNCTION__, "TEST_0");
	}

}
