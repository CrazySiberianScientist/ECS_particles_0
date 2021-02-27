#pragma once

#include "ecs/ECS.h"

namespace Common
{
	class Engine;
}

// WARNING: Don't use any VIRTUAL functional!
class BaseSystem
{
public:
	BaseSystem(Common::Engine &engine_) : engine(engine_) {}

protected:
	Common::Engine &engine;
};