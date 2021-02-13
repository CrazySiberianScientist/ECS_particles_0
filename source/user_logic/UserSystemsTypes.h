#pragma once

#include "utils/Utils.h"

namespace UserLogic
{
	// Forward declare User Systems there
	class SceneLogicSystem;
	class TestLogicSystem;

	// Add pointer of User System there
	using SystemsTypes = Utils::TypesPack<SceneLogicSystem*, TestLogicSystem*>;
}