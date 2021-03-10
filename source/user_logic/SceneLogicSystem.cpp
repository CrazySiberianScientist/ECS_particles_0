#include "SceneLogicSystem.h"
#include "engine/Engine.h"
#include <random>

void UserLogic::SceneLogicSystem::init(SystemsOrders::Init::SCENE)
{
	camera_entity = engine.createEntity();
	auto camera = engine.getComponentManager().createBundle(EngineLogic::Components::CameraBundle{}, camera_entity);
	std::get<EngineLogic::Components::Transform*>(camera)->pos[0] = -0.6f;
	engine.linkEntityToSystem<EngineLogic::CameraSystem>(camera_entity);
	engine.getSystem<EngineLogic::CameraSystem>().setMainCamera(camera_entity);

	for (auto i = 0; i < 100; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		auto rand_coord = [&gen]()
		{
			return std::generate_canonical<float, 32>(gen) - 0.5f;
		};

		auto entity = engine.createEntity();
		engine.getComponentManager().createComponent<EngineLogic::Components::Transform>(entity,
			{ glm::vec3(rand_coord(), rand_coord(), rand_coord())
			, glm::angleAxis(glm::half_pi<float>() * rand_coord(), glm::normalize(glm::vec3(rand_coord(), rand_coord(), rand_coord())))
			, glm::vec3(1.0f) });
		engine.getComponentManager().createComponent<Components::Speed>(entity, { std::max(0.1f, rand_coord()) * glm::normalize(glm::vec3(rand_coord(), rand_coord(), rand_coord())) });

		engine.linkEntityToSystem<UserLogic::TestLogicSystem>(entity);
	}
}

void UserLogic::SceneLogicSystem::update(SystemsOrders::Update::SCENE)
{
	auto t = engine.getComponentManager().getComponent<EngineLogic::Components::Transform>(camera_entity);
	t->pos = glm::angleAxis(glm::radians(30.0f * engine.getSystem<EngineLogic::AppSystem>().getIFPS()), glm::vec3(0.0, 1.0f, 0.0f)) * t->pos;
}
