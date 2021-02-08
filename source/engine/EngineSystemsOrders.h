#pragma once

#include "utils/Utils.h"

namespace Baka
{
	struct EngineSystemsOrders
	{
		UTILS_ENUM_SEQUENCE(Init, APP, RENDER);
		UTILS_ENUM_SEQUENCE(Update, APP, RENDER);
		UTILS_ENUM_SEQUENCE(Destroy, RENDER, APP);
	};
}