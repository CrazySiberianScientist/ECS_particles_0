#include "BakaEngine.h"

#include "ECSUtils.h"
#include "utils/Utils.h"

#include <iostream>

using namespace std;

struct Kaka
{
	int i = 666;
	float f = 13.0f;
};

int main()
{
	ECSUtils::ECS::ComponentManager<Kaka> cm;
	cm.create(13);
	auto c = cm.getComponent(13);
	cm.remove(13);

	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);
	return 0;
}