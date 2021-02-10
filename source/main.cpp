#include "engine/BakaEngine.h"
#include "user_logic/UserSystems.h"
#include "user_logic/UserComponents.h"

#include <iostream>

using namespace std;
using namespace Baka;


struct TestT
{
	template<typename _T>
	void f();

	template<>
	void f<UserSystemsOrders::Init>()
	{
		printf("!! %d\n", 0);
	}

	template<>
	void f<UserSystemsOrders::Update>()
	{
		printf("!! %d\n", 666);
	}

	void init() {}
};

struct TestB
{

};

template<typename _System>
struct HasInit
{
	template<typename _Type, void(_Type::*)()> struct func_pattern {};
	template<typename _Type> static constexpr std::true_type check_func(func_pattern<_Type, &_Type::init>*);
	template<typename _Type> static constexpr std::false_type check_func(...);
	static constexpr auto value = std::is_same<decltype(check_func<_System>(nullptr)), std::true_type>::value;
};

template<typename _System, typename _OrderType>
struct HasF
{
	template<typename _Type, void(_Type::*)()> struct func_pattern {};
	template<typename _Type> static constexpr std::true_type check_func(func_pattern<_Type, &_Type::template f<_OrderType>>*);
	template<typename _Type> static constexpr std::false_type check_func(...);
	static constexpr auto value = std::is_same<decltype(check_func<_System>(nullptr)), std::true_type>::value;
};

int main()
{
	auto ptr = &TestT::f<UserSystemsOrders::Init>;

	TestT test_t;
	constexpr auto v = HasF<TestT, UserSystemsOrders::Init>::value;

	printf("!! %d %d\n", HasInit<TestT>::value != 0, HasInit<TestB>::value != 0);

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