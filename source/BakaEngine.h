#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct GLFWwindow;

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

		static void glfw_error_callback(int error, const char* description);
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}