#pragma once

#include "engine/BaseSystem.h"
#include "UserSystemsOrders.h"

namespace UserLogic
{

	class TestLogicSystem : public BaseSystem
	{
	public:
		void init(SystemsOrders::Init::TEST_0)
		{
			printf("!! %s %s\n", __FUNCTION__, "TEST_0");
		}

		void init(SystemsOrders::Init::SCENE_0)
		{
			printf("!! %s %s\n", __FUNCTION__, "SCENE_0");
		}

		void init(SystemsOrders::Init::TEST_1)
		{
			printf("!! %s %s\n", __FUNCTION__, "TEST_1");
		}
	};

}