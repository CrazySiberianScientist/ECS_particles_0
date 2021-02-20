#include "engine/BakaEngine.h"
#include <iostream>


using namespace std;
//using namespace UserLogic;
//using namespace Common;

int main()
{
	Common::Engine engine;

	//constexpr auto v = Common::Engine::SystemInfo<TestLogicSystem*>::init_methods_count;
	//constexpr auto v = Common::Engine::has_init<UserLogic::TestLogicSystem, UserLogic::SystemsOrders::Init::TEST_0>::value;
	//printf("!! %d\n", v);


	auto entity = engine.createEntity();
	engine.linkEntityToSystem<UserLogic::TestLogicSystem>(entity);
	engine.linkEntityToSystem<EngineLogic::AppSystem>(entity);

	constexpr auto n = Common::SystemsTypes::getTypeIndex<UserLogic::SceneLogicSystem>();
	//constexpr auto n = Common::SystemsTypes::getTypeIndex<int>();

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