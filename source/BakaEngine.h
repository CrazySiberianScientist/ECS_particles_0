#pragma once

#include <GLFW/glfw3.h>

namespace Baka
{
	class Engine
	{
	public:
		void run();

	private:
		void init();
		void update();
		void destroy();
	};
}