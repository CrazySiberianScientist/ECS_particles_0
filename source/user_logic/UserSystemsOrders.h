#pragma once

#include "engine/EngineSystemsOrders.h"

namespace UserLogic
{
	struct SystemsOrders
	{
		struct Init
		{
			SYSTEMS_ORDER(SCENE);
			SYSTEMS_ORDER(TEST_0);
			SYSTEMS_ORDERS_TYPES(SCENE, TEST_0);
		};

		struct Update
		{
			SYSTEMS_ORDER(SCENE);
			SYSTEMS_ORDER(TEST_0);
			SYSTEMS_ORDER(TEST_TRIANGLE);
			SYSTEMS_ORDERS_TYPES(SCENE, TEST_0);
		};

		struct PostUpdate
		{
			SYSTEMS_ORDERS_TYPES();
		};

		struct Destroy
		{
			SYSTEMS_ORDER(SCENE);
			SYSTEMS_ORDER(TEST_0);
			SYSTEMS_ORDERS_TYPES(SCENE, TEST_0);
		};
	};
}