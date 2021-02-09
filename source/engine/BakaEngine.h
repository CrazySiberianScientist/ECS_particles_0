#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/gl.h>
#include <gl/GL.h>

#include "ecs/ECS.h"
#include "EngineComponents.h"
#include "EngineSystems.h"

struct GLFWwindow;

namespace Baka
{
	template<typename _UserComponentsPack = Utils::TypesPack<>
		, typename _UserSystemsPack = Utils::TypesPack<>
		, typename _UserSystemsInitOrders = Utils::TypesPack<>
		, typename _UserSystemsUpdateOrders = Utils::TypesPack<>
		, typename _UserSystemsDestroyOrders = Utils::TypesPack<>>
	class Engine
	{
		using ComponentManagerType = decltype(Utils::combineTypesPack<ECS::ComponentManager>(EngineComponents::ComponentsTypes{}, _UserComponentsPack{}));
		using SystemsCollection = decltype(Utils::combineTypesPack<std::tuple>(EngineSystemsTypes{}, _UserSystemsPack{}));
		using SystemsInitOrders = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineSystemsOrders::Init::types{}, _UserSystemsInitOrders{}));
		using SystemsUpdateOrders = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineSystemsOrders::Update::types{}, _UserSystemsUpdateOrders{}));
		using SystemsDestroyOrders = decltype(Utils::combineTypesPack<Utils::TypesPack>(EngineSystemsOrders::Destroy::types{}, _UserSystemsDestroyOrders{}));

	public:
		Engine() {}
		void run() {}
		
		template<typename _System>
		const auto &getSystem() { return std::get<_System>(systems); }

		auto createEntity() { return entity_manager.create(); }
		void removeEnity(const ECS::EntityIdType entity_id)
		{
			component_manager.removeAllComponents(entity_id);
			entity_manager.remove(entity_id);
		}

		auto &getComponentManager() { return component_manager; }
		

	private:
		static void glfw_error_callback(int error, const char* description) {}
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){}

		

	private:
		ECS::EntityManager entity_manager;
		ComponentManagerType component_manager;
		SystemsCollection systems;
	};


	/*void Engine::glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	void Engine::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	Engine::Engine()
	{
		glfwSetErrorCallback(glfw_error_callback);
	}

	void Engine::run(LogicBase *logic)
	{
		if (!logic) return;

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
	}*/
}