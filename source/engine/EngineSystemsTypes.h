#pragma once

#include "utils/Utils.h"

namespace EngineLogic
{
	class AppSystem;
	class CameraSystem;

	using SystemsTypes = Utils::TypesPack<AppSystem*, CameraSystem*>;
}