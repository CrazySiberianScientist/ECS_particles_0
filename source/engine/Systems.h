#pragma once

#include "EngineSystems.h"
#include "user_logic/UserSystems.h"

#include <bitset>

namespace Common
{
	using SystemsTypes = decltype(Utils::conCatTypesPack(EngineLogic::SystemsTypes{}, UserLogic::SystemsTypes{}));
}