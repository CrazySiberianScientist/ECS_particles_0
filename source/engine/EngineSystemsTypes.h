#pragma once

#include "utils/Utils.h"

namespace Baka
{
	class AppSystem;
	class CameraSystem;

	using EngineSystemsTypes = Utils::TypesPack<AppSystem*, CameraSystem*>;
}