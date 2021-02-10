#include "engine/BakaEngine.h"
#include "user_logic/UserSystems.h"
#include "user_logic/UserComponents.h"

#include <iostream>

using namespace std;
using namespace Baka;


struct TestT
{
	void init(UserSystemsOrders::Init::TEST_0)
	{
		printf("!! %s %s\n", __FUNCTION__, "TEST_0");
	}
	void init(UserSystemsOrders::Init::TEST_1)
	{
		printf("!! %d\n", 666);
	}
};


#define DECLARE_METHOD_CHECKER(METHOD_NAME)\
template<typename _System, typename _OrderType>\
struct has_##METHOD_NAME {\
	template<typename _Type, void(_Type::*)(_OrderType)> struct func_pattern {};\
	template<typename _Type> static constexpr std::true_type check_func(func_pattern<_Type, &_Type::METHOD_NAME>*);\
	template<typename _Type> static constexpr std::false_type check_func(...);\
	static constexpr auto value = std::is_same<decltype(check_func<_System>(nullptr)), std::true_type>::value; }

DECLARE_METHOD_CHECKER(init);

#undef DECLARE_METHOD_CHECKER



int main()
{
	//auto ptr = &TestT::f<UserSystemsOrders::Init>;
	TestT test_t;
	constexpr auto v = has_init<TestT, UserSystemsOrders::Init::TEST_1>::value;

	printf("!! %d %d\n", has_init<TestT, UserSystemsOrders::Init::SCENE_0>::value != 0, has_init<TestT, UserSystemsOrders::Init::TEST_0>::value != 0);

	Engine<UserComponents::ComponentsTypes
		, UserSystemsTypes
		, UserSystemsOrders::Init::types
		, UserSystemsOrders::Update::types
		, UserSystemsOrders::Destroy::types> engine;


	engine.getSystem<CameraSystem>();

	auto e = engine.createEntity();
	engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e, {}, {});
	auto cb = engine.getComponentManager().createBundle(EngineComponents::CameraBundle_v,
		e);

	//Baka::Engine().run(new Logic);
	return 0;
}