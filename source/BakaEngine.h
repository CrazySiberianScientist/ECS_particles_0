#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace Baka
{
	class LogicBase
	{
	public:
		virtual ~LogicBase() {}
		virtual void init() {}
		virtual void update() {}
		virtual void destroy() {}
	};

	class Engine
	{
	public:
		Engine();
		void run(LogicBase *logic);

	private:
		static void glfw_error_callback(int error, const char* description);
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}