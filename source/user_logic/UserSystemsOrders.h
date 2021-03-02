#pragma once

#include "engine/EngineSystemsOrders.h"

namespace UserLogic
{
	struct SystemsOrders
	{
		struct Init
		{
			SYSTEMS_ORDER(SCENE_0);
			SYSTEMS_ORDER(TEST_0);
			SYSTEMS_ORDER(SCENE_1);
			SYSTEMS_ORDER(TEST_1);
			SYSTEMS_ORDERS_TYPES(SCENE_0, TEST_0, SCENE_1, TEST_1);
		};

		struct Update
		{
			SYSTEMS_ORDER(SCENE_0);
			SYSTEMS_ORDER(TEST_0);
			SYSTEMS_ORDER(SCENE_1);
			SYSTEMS_ORDER(TEST_1);
			SYSTEMS_ORDERS_TYPES(SCENE_0, TEST_0, SCENE_1, TEST_1);
		};

		struct PostUpdate
		{

		};

		struct Destroy
		{
			SYSTEMS_ORDER(SCENE_0);
			SYSTEMS_ORDER(TEST_0);
			SYSTEMS_ORDER(SCENE_1);
			SYSTEMS_ORDER(TEST_1);
			SYSTEMS_ORDERS_TYPES(SCENE_0, TEST_0, SCENE_1, TEST_1);
		};
	};
}