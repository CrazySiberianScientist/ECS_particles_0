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

	auto entity = engine.createEntity();
	engine.linkEntityToSystem<TestLogicSystem>(entity);

	/*engine.getSystem<CameraSystem>();

	auto e = engine.createEntity();
	engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	auto cb = engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e);

	engine.run();*/

	return 0;
}