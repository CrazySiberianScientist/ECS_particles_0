#pragma once

#include "EngineSystemsOrders.h"
#include "user_logic/UserSystemsOrders.h"

namespace Baka
{
	struct SystemsOrders
	{
		using Init = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineSystemsOrders::Init::types{}, UserSystemsOrders::Init::types{}));
		using Update = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineSystemsOrders::Update::types{}, UserSystemsOrders::Update::types{}));
		using Destroy = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineSystemsOrders::Destroy::types{}, UserSystemsOrders::Destroy::types{}));
	};
}