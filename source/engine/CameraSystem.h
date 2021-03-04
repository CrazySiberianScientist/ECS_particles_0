#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

namespace EngineLogic
{
	class CameraSystem : public BaseSystem
	{
	public:
		CameraSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void setMainCamera(const ECS::EntityIdType camera_id) { main_camera_id = camera_id; }
		auto getMainCamera() const { return main_camera_id; }

	private:
		ECS::EntityIdType main_camera_id = ECS::EntityIdType_Invalid;
	};
}