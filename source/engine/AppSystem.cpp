#include "AppSystem.h"

#include <glad/gl.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

#include "engine/Engine.h"

namespace EngineLogic
{
	void AppSystem::init(SystemsOrders::Init::APP)
	{
		if (!glfwInit())
		{
			std::cerr << "[Error] glfwInit() - failed to init" << std::endl;
			engine.stop();
			return;
		}

		window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
		if (!window)
		{
			std::cerr << "[Error] glfwCreateWindow() - failed to create window" << std::endl;
			engine.stop();
			return;
		}

		glfwSetKeyCallback(window, glfw_key_callback);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		gladLoadGL(glfwGetProcAddress);
	}

	void AppSystem::update(SystemsOrders::Update::APP_FRAME_BEGIN)
	{
		if (glfwWindowShouldClose(window))
		{
			engine.stop();
			return;
		}

		glfwGetFramebufferSize(window, &frame_size[0], &frame_size[1]);
		glViewport(0, 0, frame_size[0], frame_size[1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		time_state.ifps = static_cast<float>(glfwGetTime() - time_state.last_time);
		time_state.last_time = glfwGetTime();
	}

	void AppSystem::postUpdate(SystemsOrders::PostUpdate::APP_FRAME_END)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void AppSystem::destroy(SystemsOrders::Destroy::APP)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void AppSystem::glfw_error_callback(int error, const char * description)
	{
		std::cerr << "[GLFW Error] (" << error << ") " << description << std::endl;
	}

	void AppSystem::glfw_key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
	}

}
