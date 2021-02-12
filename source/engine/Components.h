#pragma once

#include "EngineComponents.h"
#include "user_logic/UserComponents.h"

namespace Common
{
	using ComponentsTypes = decltype(Utils::conCatTypesPack(EngineLogic::Components::ComponentsTypes{}, UserLogic::Components::ComponentsTypes{}));
}