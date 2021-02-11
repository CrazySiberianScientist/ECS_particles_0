#pragma once

#include "engine/BaseSystem.h"
#include "UserSystemsOrders.h"

class SceneLogicSystem : public BaseSystem
{
public:
	void init(UserSystemsOrders::Init::SCENE_0)
	{
		printf("!! %s %s\n", __FUNCTION__, "SCENE_0");
	}

	void init(UserSystemsOrders::Init::SCENE_1)
	{
		printf("!! %s %s\n", __FUNCTION__, "SCENE_1");
	}
};