#pragma once

#include "EngineSystems.h"
#include "user_logic/UserSystems.h"

namespace Common
{
	using SystemsTypes = decltype(Utils::conCatTypesPack(EngineLogic::SystemsTypes{}, UserLogic::SystemsTypes{}));
}