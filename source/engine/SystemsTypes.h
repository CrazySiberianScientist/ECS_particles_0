#pragma once

#include "EngineSystemsTypes.h"
#include "user_logic/UserSystemsTypes.h"

namespace Baka
{
	using SystemsTypes = decltype(Utils::conCatTypesPack(EngineSystemsTypes{}, UserSystemsTypes{}));
}