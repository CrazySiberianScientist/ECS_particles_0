#pragma once

// Include Engine Systems here

#include "AppSystem.h"
#include "ShadersSystem.h"
#include "CameraSystem.h"

namespace EngineLogic
{
	// Add Engine System Type here
	using SystemsTypes = Utils::TypesPack<AppSystem, ShadersSystem, CameraSystem>;
}