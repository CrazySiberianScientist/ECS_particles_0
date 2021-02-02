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
		Utils::ChunkTable<Kaka, 2> test_buffer;
		std::vector<size_t> keys;
		for (auto i = 0; i < 100; ++i)
		{
			size_t v = rand() % 100;
			keys.push_back(v);
			test_buffer.emplace(v, v, v / 100.0f);
		}
		for (auto k : keys)
			if (k == test_buffer.get(k)->i)
				printf("KAKAKAKA!\n");
	}

	class Logic : public Baka::LogicBase
	{

	};

	Baka::Engine().run(new Logic);
	return 0;
}