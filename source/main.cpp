#include "BakaEngine.h"

//#include "ECSUtils.h"
#include "utils/Utils.h"

#include <iostream>

using namespace std;

struct Kaka
{
	Kaka(int i_ = 666, float f_ = 100.0f) 
		: i(i_)
		, f(f_)
	{
		printf("Kaka(%d, %f)\n", i, f);
	}

	~Kaka()
	{
		printf("~Kaka(%d)\n", i);
	}

	int i = 666;
	float f = 100.0f;
};

int main()
{
	//ECSUtils::ECS<100'000, 100> ecs;
	{
		Utils::ChunkBuffer<Kaka, 2> test_buffer;
		std::vector<Kaka*> kaki;
		for (auto i = 0; i < 100; ++i)
		{
			//kaki.push_back(test_buffer.emplace_back(i, 100 - i));
			//test_buffer.remove(test_buffer.emplace_back(i, 100 - i));
			test_buffer.emplace_back(i, 100 - i);
		}
		/*for (auto k : kaki)
			test_buffer.remove(k);*/

		auto kaka = test_buffer.emplace_back();
		//test_buffer.remove(kaka);
	}

	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);
	return 0;
}