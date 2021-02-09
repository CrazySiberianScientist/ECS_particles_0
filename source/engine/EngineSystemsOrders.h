#pragma once

#include "utils/Utils.h"

namespace Baka
{
	#define BAKA_SYSTEMS_ORDER(NAME) struct NAME{};
	#define BAKA_SYSTEMS_ORDERS_TYPES(...) using types = Utils::TypesPack<__VA_ARGS__>;

	struct EngineSystemsOrders
	{
		struct Init
		{
			BAKA_SYSTEMS_ORDER(APP);
			BAKA_SYSTEMS_ORDER(RENDER);
			BAKA_SYSTEMS_ORDERS_TYPES(APP, RENDER);
		};

		struct Update
		{
			BAKA_SYSTEMS_ORDER(APP);
			BAKA_SYSTEMS_ORDER(RENDER);
			BAKA_SYSTEMS_ORDERS_TYPES(APP, RENDER);
		};

		struct Destroy
		{
			BAKA_SYSTEMS_ORDER(RENDER);
			BAKA_SYSTEMS_ORDER(APP);
			BAKA_SYSTEMS_ORDERS_TYPES(RENDER, APP);
		};
	};
}