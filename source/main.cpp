#include "BakaEngine.h"

#include "ECSUtils.h"

int main()
{
	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);

	return 0;
}