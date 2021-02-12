#pragma once

#include "EngineSystemsTypes.h"
#include "user_logic/UserSystemsTypes.h"

namespace Common
{
	using SystemsTypes = decltype(Utils::conCatTypesPack(EngineLogic::SystemsTypes{}, UserLogic::SystemsTypes{}));
}