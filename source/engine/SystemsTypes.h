#pragma once

#include "EngineSystemsTypes.h"
#include "user_logic/UserSystemsTypes.h"

using SystemsTypes = decltype(Utils::conCatTypesPack(EngineLogic::SystemsTypes{}, UserLogic::SystemsTypes{}));