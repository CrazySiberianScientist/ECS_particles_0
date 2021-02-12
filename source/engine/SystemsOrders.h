#pragma once

#include "EngineSystemsOrders.h"
#include "user_logic/UserSystemsOrders.h"

namespace Common
{
	struct SystemsOrders
	{
		using Init = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineLogic::SystemsOrders::Init::types{}, UserLogic::SystemsOrders::Init::types{}));
		using Update = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineLogic::SystemsOrders::Update::types{}, UserLogic::SystemsOrders::Update::types{}));
		using Destroy = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineLogic::SystemsOrders::Destroy::types{}, UserLogic::SystemsOrders::Destroy::types{}));
	};
}