#include "engine/BakaEngine.h"
#include "user_logic/UserSystems.h"
#include "user_logic/UserComponents.h"

#include <iostream>

using namespace std;
using namespace Baka;

int main()
{
	Engine<UserComponents::ComponentsTypes> engine;


	/*engine.getSystem<CameraSystem>();

	auto e = engine.createEntity();
	engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	auto cb = engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e);

	engine.run();*/

	return 0;
}