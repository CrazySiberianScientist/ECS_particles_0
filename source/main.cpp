#include "BakaEngine.h"

#include "ECSUtils.h"

int main()
{
	ECSUtils::PreferredIntegralType<UINT64_MAX>::type v;

	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);

	return 0;
}