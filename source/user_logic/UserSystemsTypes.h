#pragma once

#include "utils/Utils.h"

namespace UserLogic
{
	class SceneLogicSystem;
	class TestLogicSystem;

	using SystemsTypes = Utils::TypesPack<SceneLogicSystem*, TestLogicSystem*>;
}