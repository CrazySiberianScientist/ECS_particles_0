#pragma once

#include "utils/Utils.h"

class SceneLogicSystem;
class TestLogicSystem;

namespace UserLogic
{

	using SystemsTypes = Utils::TypesPack<SceneLogicSystem*, TestLogicSystem*>;

}