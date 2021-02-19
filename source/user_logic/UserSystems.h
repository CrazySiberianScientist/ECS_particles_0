#pragma once

// Include User Systems here

#include "SceneLogicSystem.h"
#include "TestLogicSystem.h"

namespace UserLogic
{
	// Add User System Type here
	using SystemsTypes = Utils::TypesPack<SceneLogicSystem, TestLogicSystem>;
}