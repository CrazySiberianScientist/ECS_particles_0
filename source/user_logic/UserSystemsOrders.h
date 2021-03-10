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
			SYSTEMS_ORDER(TEST_PARTICLES);
			SYSTEMS_ORDER(TEST_PARTICLES_MOVE);
			SYSTEMS_ORDERS_TYPES(SCENE, TEST_0, TEST_PARTICLES, TEST_PARTICLES_MOVE);
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