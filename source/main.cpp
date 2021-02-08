#include "engine/BakaEngine.h"

#include <iostream>

using namespace std;
using namespace Baka;

struct TestC
{
	bool satan = false;
};

struct TestT
{
	template<size_t _Order>
	void f();

	template<>
	void f<0>()
	{
		printf("!! %d\n", 0);
	}

	template<>
	void f<666>()
	{
		printf("!! %d\n", 666);
	}
};

UTILS_ENUM_SEQUENCE(InitOrders, BASIC, OTHER, BAKA);

int main()
{
	TestT test_t;
	test_t.f<0>();
	test_t.f<666>();
	//test_t.f<1>();

	class Logic : public LogicBase
	{

	};

	Engine<Utils::TypesPack<TestC>> engine;
	auto e = engine.createEntity();
	engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	auto cb = engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e);
	engine.getComponentManager().createComponent<TestC>(e);

	//Baka::Engine().run(new Logic);
	return 0;
}