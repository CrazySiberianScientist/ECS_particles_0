#include "BakaEngine.h"

//#include "ECSUtils.h"
#include "utils/Utils.h"

#include <iostream>

using namespace std;

int main()
{
	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);
	return 0;
}