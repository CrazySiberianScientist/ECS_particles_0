#pragma once

#include "utils/Utils.h"

namespace EngineLogic
{
	// Forward declare Engine Systems there
	class AppSystem;
	class CameraSystem;

	// Add pointer of Engine System there
	using SystemsTypes = Utils::TypesPack<AppSystem*, CameraSystem*>;
}