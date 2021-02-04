#include "BakaEngine.h"

#include "ECSUtils.h"
#include "utils/Utils.h"

#include <iostream>

using namespace std;

struct Kaka
{
	int i = 666;
	float f = 13.0f ;
};

struct Gaka
{
	string s = "LOH";
};

int main()
{
	constexpr auto i_t = std::is_constructible<Kaka, int, float>::value;

	ECSUtils::EntityManager em;
	ECSUtils::ComponentManager<Kaka, Gaka> cm;
	auto e = em.create();
	cm.createComponent<Kaka>(e, 6, 13.0f);
	cm.createComponent<Gaka>(e, "TEST");
	cm.removeAllComponents(e);
	

	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);
	return 0;
}