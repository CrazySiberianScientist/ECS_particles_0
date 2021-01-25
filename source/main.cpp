#include "BakaEngine.h"

#include "ECSUtils.h"

int main()
{
	constexpr auto v = ECSUtils::PreferredIntegralType<UINT64_MAX>::value_to_index();

	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);

	return 0;
}