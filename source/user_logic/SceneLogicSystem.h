#pragma once

#include "engine/BaseSystem.h"
#include "UserSystemsOrders.h"

namespace UserLogic
{
	class SceneLogicSystem : public BaseSystem
	{
	public:
		SceneLogicSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void init(SystemsOrders::Init::SCENE_0);
	};

}