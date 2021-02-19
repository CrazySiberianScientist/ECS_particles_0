#pragma once

// Include Engine Systems here

#include "AppSystem.h"
#include "CameraSystem.h"

namespace EngineLogic
{
	// Add Engine System Type here
	using SystemsTypes = Utils::TypesPack<AppSystem, CameraSystem>;
}