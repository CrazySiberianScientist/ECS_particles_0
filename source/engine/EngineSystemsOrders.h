#pragma once

#include "utils/Utils.h"

namespace EngineLogic
{
	#define SYSTEMS_ORDER(NAME) struct NAME{};
	#define SYSTEMS_ORDERS_TYPES(...) using types = Utils::TypesPack<__VA_ARGS__>;

	struct SystemsOrders
	{
		struct Init
		{
			SYSTEMS_ORDER(APP);
			SYSTEMS_ORDER(RENDER);
			SYSTEMS_ORDERS_TYPES(APP, RENDER);
		};

		struct Update
		{
			SYSTEMS_ORDER(APP);
			SYSTEMS_ORDER(RENDER);
			SYSTEMS_ORDERS_TYPES(APP, RENDER);
		};

		struct Destroy
		{
			SYSTEMS_ORDER(RENDER);
			SYSTEMS_ORDER(APP);
			SYSTEMS_ORDERS_TYPES(RENDER, APP);
		};
	};
}