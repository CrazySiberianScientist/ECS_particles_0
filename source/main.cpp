#include "engine/BakaEngine.h"
#include <iostream>

using namespace std;
using namespace EngineLogic;

int main()
{
	Common::Engine engine;
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