#include "CameraSystem.h"
#include "Engine.h"

namespace EngineLogic
{
	void CameraSystem::update(SystemsOrders::Update::CAMERA, const ECS::EntityIdType camera_id)
	{
		const auto * const t = engine.getComponentManager().getComponent<Components::Transform>(camera_id);
		if (!t) return;
		auto * const ct = engine.getComponentManager().getComponent<Components::CameraTransform>(camera_id);
		if (!ct) return;

		ct->view = glm::lookAt(t->pos, ct->target_pos, glm::vec3(0.0f, 1.0f, 0.0f));

		const auto frame_aspect = engine.getSystem<EngineLogic::AppSystem>().getFrameAspect();
		ct->projection = glm::perspective(glm::radians(ct->fov_y), frame_aspect, ct->z_near, ct->z_far);
	}
}
