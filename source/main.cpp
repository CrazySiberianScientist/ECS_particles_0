#include "BakaEngine.h"

#include "ECSUtils.h"

int main()
{
	ECSUtils::ECS<100'000, 100> ecs;

	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);

	return 0;
}