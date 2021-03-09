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

void UserLogic::SceneLogicSystem::update(SystemsOrders::Update::SCENE)
{
	auto t = engine.getComponentManager().getComponent<EngineLogic::Components::Transform>(camera_entity);

	t->pos = glm::angleAxis(glm::radians(30.0f * engine.getSystem<EngineLogic::AppSystem>().getIFPS()), glm::vec3(0.0, 1.0f, 0.0f)) * t->pos;
}
