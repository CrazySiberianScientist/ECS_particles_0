#include "BakaEngine.h"

#include "ECS.h"
#include "utils/Utils.h"

#include <iostream>

using namespace std;
using namespace Baka;


int main()
{
	class Logic : public LogicBase
	{

	};

	Engine engine;
	auto e = engine.createEntity();
	engine.component_manager.createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	engine.component_manager.createBundle(EngineComponents::CameraBundle_v,
		e);
	//engine.component_manager.createComponent<EngineComponents::Transform>(e);

	//Baka::Engine().run(new Logic);
	return 0;
}