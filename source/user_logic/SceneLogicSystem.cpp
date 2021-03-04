#include "SceneLogicSystem.h"
#include "engine/Engine.h"

void UserLogic::SceneLogicSystem::init(SystemsOrders::Init::SCENE)
{
	camera_entity = engine.createEntity();
	auto camera = engine.getComponentManager().createBundle(EngineLogic::Components::CameraBundle{}, camera_entity);
	std::get<EngineLogic::Components::Transform*>(camera)->pos[0] = -2.0f;
	engine.linkEntityToSystem<EngineLogic::CameraSystem>(camera_entity);
	engine.getSystem<EngineLogic::CameraSystem>().setMainCamera(camera_entity);
}
