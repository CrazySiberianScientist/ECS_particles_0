#include "engine/BakaEngine.h"
#include <iostream>

#include "engine/EngineSystems.h"

using namespace std;

int main()
{
	Common::Engine engine;
	engine.run();

	//const auto v = Common::Engine::SystemInfo<UserLogic::TestLogicSystem>::has_init_methods;
	const auto v = Common::Engine::has_init<UserLogic::TestLogicSystem, UserLogic::SystemsOrders::Init::TEST_0>::value;
	printf("!! %d\n", v);

	/*engine.getSystem<CameraSystem>();

	auto e = engine.createEntity();
	engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	auto cb = engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e);

	engine.run();*/

	return 0;
}