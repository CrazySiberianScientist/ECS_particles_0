#include "AppSystem.h"

#include <glad/gl.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
#include <cassert>

namespace EngineLogic
{

	void AppSystem::init(SystemsOrders::Init::APP)
	{
		assert((glfwInit()) && "glfwInit() - failed to init");

		window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
		assert((window) && "glfwCreateWindow() - failed to create window");

		glfwSetKeyCallback(window, glfw_key_callback);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		gladLoadGL(glfwGetProcAddress);
	}

	void AppSystem::update(SystemsOrders::Update::APP_FRAME_BEGIN)
	{


		while (!glfwWindowShouldClose(window))
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			glfwSwapBuffers(window);
			glfwPollEvents();
		}


		return;
	}

	void AppSystem::destroy(SystemsOrders::Destroy::APP)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void AppSystem::glfw_error_callback(int error, const char * description)
	{
	}

	void AppSystem::glfw_key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
	}

}
