#include "engine/BakaEngine.h"
#include <iostream>

#include "engine/EngineSystems.h"
#include "user_logic/UserSystems.h"

using namespace std;
using namespace UserLogic;
using namespace Common;

int main()
{
	Common::Engine engine;
	engine.run();

	const auto v = Common::Engine::SystemInfo<UserLogic::TestLogicSystem>::init_methods_count;
	printf("!! %d\n", v ? 1 : 0);

	/*engine.getSystem<CameraSystem>();

	auto e = engine.createEntity();
	engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	auto cb = engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e);

	engine.run();*/

	return 0;
}