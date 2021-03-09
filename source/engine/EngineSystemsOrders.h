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
			SYSTEMS_ORDER(SHADERS);
			SYSTEMS_ORDERS_TYPES(APP, SHADERS);
		};

		struct Update
		{
			SYSTEMS_ORDER(APP_FRAME_BEGIN);
			SYSTEMS_ORDER(CAMERA);
			SYSTEMS_ORDERS_TYPES(APP_FRAME_BEGIN, CAMERA);
		};

		struct PostUpdate
		{
			SYSTEMS_ORDER(APP_FRAME_END);
			SYSTEMS_ORDERS_TYPES(APP_FRAME_END);
		};

		struct Destroy
		{
			SYSTEMS_ORDER(APP);
			SYSTEMS_ORDERS_TYPES(APP);
		};
	};
}