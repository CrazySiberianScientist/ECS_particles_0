#pragma once

#include "engine/EngineSystemsOrders.h"

struct UserSystemsOrders
{
	UTILS_ENUM_SEQUENCE(Init, APP, RENDER);
	UTILS_ENUM_SEQUENCE(Update, APP, RENDER);
	UTILS_ENUM_SEQUENCE(Destroy, RENDER, APP);
};