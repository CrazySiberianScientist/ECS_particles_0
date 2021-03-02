#include "AppSystem.h"

#include <glad/gl.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
#include <cassert>

void EngineLogic::AppSystem::init(SystemsOrders::Init::APP)
{
	assert((glfwInit()) && "glfwInit() - failed to init");

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	assert((window) && "glfwCreateWindow() - failed to create window");
	
	glfwSetKeyCallback(window, glfw_key_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	gladLoadGL(glfwGetProcAddress);
}

void EngineLogic::AppSystem::update(SystemsOrders::Update::APP_FRAME_BEGIN)
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

void EngineLogic::AppSystem::destroy(SystemsOrders::Destroy::APP)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
