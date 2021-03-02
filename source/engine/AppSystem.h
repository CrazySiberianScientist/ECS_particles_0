#pragma once

#include "EngineComponents.h"
#include "EngineSystemsOrders.h"
#include "BaseSystem.h"

struct GLFWwindow;

namespace EngineLogic
{
	class AppSystem : public BaseSystem
	{
	public:
		AppSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void init(SystemsOrders::Init::APP);
		void update(SystemsOrders::Update::APP_FRAME_BEGIN);
		void postUpdate(SystemsOrders::PostUpdate::APP_FRAME_END);
		void destroy(SystemsOrders::Destroy::APP);

	private:
		static void glfw_error_callback(int error, const char* description);
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		GLFWwindow *window = nullptr;
	};
}