#include "engine/BakaEngine.h"
#include <iostream>


using namespace std;
//using namespace UserLogic;
//using namespace Common;

int main()
{
	Common::Engine engine;

	//constexpr int v = Common::Engine::SystemInfo<UserLogic::TestLogicSystem>::destroy_methods_number;
	//constexpr auto v = Common::Engine::has_destroy<UserLogic::TestLogicSystem, UserLogic::SystemsOrders::Destroy::TEST_0>::value;
	//printf("!! %d\n", v);

	//return 0;

	auto entity = engine.createEntity();
	engine.linkEntityToSystem<UserLogic::TestLogicSystem>(entity);
	/*for (auto i = 0; i < 100; ++i)
	{
		auto entity = engine.createEntity();
		engine.linkEntityToSystem<UserLogic::TestLogicSystem>(entity);
	}*/

	engine.linkEntityToSystem<EngineLogic::AppSystem>(entity);
	engine.unlinkEntityFromSystem<EngineLogic::AppSystem>(entity);


	engine.run();

	/*engine.getSystem<CameraSystem>();

	auto e = engine.createEntity();
	engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	auto cb = engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e);

	engine.run();*/

	return 0;
}