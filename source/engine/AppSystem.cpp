#include "AppSystem.h"

#include <glad/gl.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

void EngineLogic::AppSystem::init(SystemsOrders::Init::APP)
{
	printf("!! %s %s\n", __FUNCTION__, "APP");
}

void EngineLogic::AppSystem::update(SystemsOrders::Update::APP)
{
	if (!glfwInit())
			return;

		auto window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return;
		}
		glfwSetKeyCallback(window, glfw_key_callback);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		gladLoadGL(glfwGetProcAddress);

		logic->init();

		while (!glfwWindowShouldClose(window))
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			logic->update();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		logic->destroy();

		glfwDestroyWindow(window);

		glfwTerminate();
		return;
}

void EngineLogic::AppSystem::destroy(SystemsOrders::Destroy::APP)
{
	printf("!! %s %s\n", __FUNCTION__, "APP");
}
