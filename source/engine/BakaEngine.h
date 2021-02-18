#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/gl.h>
#include <gl/GL.h>

#include "ecs/ECS.h"
#include "Components.h"
#include "SystemsTypes.h"
#include "SystemsOrders.h"

struct GLFWwindow;

namespace Common
{
	class Engine
	{
	public:
		using ComponentManagerType = decltype(Utils::convertTypesPack<ECS::ComponentManager>(ComponentsTypes{}));
		using SystemsCollection = decltype(Utils::convertTypesPack<std::tuple>(SystemsTypes{}));

		#define DECLARE_METHOD_CHECKER(METHOD_NAME)\
		template<typename _System, typename _OrderType>\
		struct has_##METHOD_NAME {\
			template<typename _Type, void(_Type::*)(_OrderType, const ECS::EntityIdType)> struct func_pattern {};\
			template<typename _Type> static constexpr std::true_type check_func(func_pattern<_Type, &_Type::METHOD_NAME>*);\
			template<typename _Type> static constexpr std::false_type check_func(...);\
			static constexpr auto value = std::is_same<decltype(check_func<_System>(nullptr)), std::true_type>::value; }
		DECLARE_METHOD_CHECKER(init);
		DECLARE_METHOD_CHECKER(update);
		DECLARE_METHOD_CHECKER(destroy);
		#undef DECLARE_METHOD_CHECKER

	private:
		template<typename _System>
		struct SystemInfo
		{
			template<typename ..._Orders>
			static constexpr size_t check_inits(Utils::TypesPack<_Orders...>) { return (has_init<_System, _Orders>::value + ...); }
			static constexpr auto init_methods_count = check_inits(Common::SystemsOrders::Init{});

			bool has_entity(const ECS::EntityIdType entity_id) const
			{
				if (auto found_it = std::find(entities.begin(), entities.end(), entity_id);
					found_it != entities.end()) return true;
				if (auto found_it = std::find(not_inited_entities.begin(), not_inited_entities.end(), entity_id);
					found_it != not_inited_entities.end()) return true;
				if (auto found_it = std::find(initing_entities.begin(), initing_entities.end(), entity_id);
					found_it != initing_entities.end()) return true;
				return false;
			}

			std::vector<ECS::EntityIdType> entities;
			std::vector<ECS::EntityIdType> not_inited_entities;
			std::vector<ECS::EntityIdType> initing_entities;
			uint32_t passed_inits = 0;
		};
		using SystemsInfoCollection = decltype(Utils::wrapTypesPack<std::tuple, SystemInfo>(SystemsTypes{}));

	public:
		Engine();
		~Engine();
		void run();
		
		template<typename _System>
		auto *getSystem() { return std::get<_System*>(systems); }

		auto createEntity() { return entity_manager.create(); }
		void removeEnity(const ECS::EntityIdType entity_id)
		{
			component_manager.removeAllComponents(entity_id);
			entity_manager.remove(entity_id);
		}

		auto &getComponentManager() { return component_manager; }

		
		template<typename _System>
		void linkEntityToSystem(const ECS::EntityIdType entity_id)
		{
			auto &system_info = std::get<SystemInfo<_System*>>(systems_info);
			if (system_info.has_entity(entity_id)) return;
			return system_info.not_inited_entities.push_back(entity_id);
		}
		

	private:
		template <typename ..._Systems>
		void run_systems_preinits(Utils::TypesPack<_Systems...>);
		template<typename _SystemPtr>
		void run_system_preinit();

		template<typename ..._Orders>
		void run_inits_orders(Utils::TypesPack<_Orders...>);
		template <typename _Order, typename ..._Systems>
		void run_systems_inits(Utils::TypesPack<_Systems...>);
		template<typename _SystemPtr, typename _Order>
		void run_system_init();

		void construct_systems();
		template<typename ..._Systems>
		void construct_systems_impl(std::tuple<_Systems...>);
		template<typename _SystemPtr>
		void construct_system();

		void destruct_systems();
		template<typename ..._Systems>
		void destruct_systems_impl(std::tuple<_Systems...>);
		template<typename _SystemPtr>
		void destruct_system();

	private:
		ECS::EntityManager entity_manager;
		ComponentManagerType component_manager;
		SystemsCollection systems;
		SystemsInfoCollection systems_info;
	};

	/*static void glfw_error_callback(int error, const char* description) {}
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){}*/

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


