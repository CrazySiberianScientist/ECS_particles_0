#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/gl.h>
#include <gl/GL.h>

#include "ecs/ECS.h"
#include "Components.h"
#include "SystemsOrders.h"
#include "Systems.h"

struct GLFWwindow;

namespace Common
{
	class Engine
	{
	private:
		using ComponentManagerType = decltype(Utils::convertTypesPack<ECS::ComponentManager>(ComponentsTypes{}));
		using SystemsCollection = decltype(Utils::convertTypesPack<std::tuple>(convertTypesToPointersPack(SystemsTypes{})));

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
			enum EntityState
			{
				TO_INIT
				, INITING
				, UPDATE
				, TO_DESTROY
				, DESTROYING
				COUNT
			};

			void linkEntity(const ECS::EntityIdType entity_id)
			{
				if constexpr (SystemInfo<_SystemPtr>::init_methods_count != 0)
				{
					entities_queues[EntityState::TO_INIT].push_back(entity_id);
					*(entity_states.emplace(entity_id)) = EntityState::TO_INIT;
				}
				else entities_queues[EntityState::UPDATE].push_back(entity_id);
			}

			void unlinkEntity(const ECS::EntityIdType entity_id)
			{
				auto &state = *(entity_states.get(entity_id));
				if (state != EntityState::Update)
				{
					std::cerr << "[Warning] " << __func__ << " - Entity(ID " << entity_id << ") linked to System(Index " 
						<< SystemsTypes::getTypeIndex<_System>() << ") must be in UPDATE state for Unlinking" << std::endl;
					return;
				}
				state = EntityState::TO_DESTROY;
				entities_queues[EntityState::TO_DESTROY].push_back(entity_id);
			}


			template<typename ..._Orders>
			static constexpr size_t check_inits(Utils::TypesPack<_Orders...>) { return (has_init<_System, _Orders>::value + ...); }
			static constexpr auto init_methods_count = check_inits(Common::SystemsOrders::Init{});
			template<typename ..._Orders>
			static constexpr size_t check_destroys(Utils::TypesPack<_Orders...>) { return (has_destroy<_System, _Orders>::value + ...); }
			static constexpr auto destroy_methods_count = check_destroys(Common::SystemsOrders::Destroy{});

			std::vector<ECS::EntityIdType> entities_queues[EntityState::COUNT];
			Utils::ChunkTable<uint8_t> entity_states;

			uint32_t passed_inits = 0;
			uint32_t passed_destroy = 0;
		};
		using SystemsInfoCollection = decltype(Utils::wrapTypesPack<std::tuple, SystemInfo>(SystemsTypes{}));

	public:
		Engine();
		~Engine();
		void run();
		void stop();

		auto createEntity()
		{
			const auto entity_id = entity_manager.create();
			if (entity_id == ECS::EntityIdType_Invalid) std::cerr << "[Warning] " << __func__ << " - Cannot create new Entity, maximum count" << std::endl;
			else entity_systems_masks.emplace(entity_id);
			return entity_id;
		}
		void removeEnity(const ECS::EntityIdType entity_id)
		{
			if (entity_id == ECS::EntityIdType_Invalid)
			{
				std::cerr << "[Warning] " << __func__ << " - entity_id is invalid" << std::endl;
				return;
			}

			// TODO: order
			/*component_manager.removeAllComponents(entity_id);
			entity_manager.remove(entity_id);*/
		}

		auto &getComponentManager() { return component_manager; }
		
		template<typename _System>
		auto *getSystem() { return std::get<_System*>(systems); }
		
		template<typename _System>
		void linkEntityToSystem(const ECS::EntityIdType entity_id)
		{
			auto mask = entity_systems_masks.get(entity_id);
			if (!mask)
			{
				std::cerr << "[Warning] " << __func__ << " - Entity(ID " << entity_id << ") isn't exsist" << std::endl;
				return;
			}
			if ((*mask)[SystemsTypes::getTypeIndex<_System>()])
			{
				std::cerr << "[Warning] " << __func__ << " - Entity(ID " << entity_id << ") is already linked to System(Index " 
					<< SystemsTypes::getTypeIndex<_System>() << ")" << std::endl;
				return;
			}
			(*mask)[SystemsTypes::getTypeIndex<_System>()] = true;

			std::get<SystemInfo<_System>>(systems_info).linkEntity(entity_id);
		}

		template<typename _System>
		void unlinkEntityFromSystem(const ECS::EntityIdType entity_id)
		{
			auto mask = entity_systems_masks.get(entity_id);
			if (!mask)
			{
				std::cerr << "[Warning] " << __func__ << " - Entity(ID " << entity_id << ") isn't exsist" << std::endl;
				return;
			}
			if (!(*mask)[SystemsTypes::getTypeIndex<_System>()])
			{
				std::cerr << "[Warning] " << __func__ << " - Entity(ID " << entity_id << ") isn't linked to System(Index " 
					<< SystemsTypes::getTypeIndex<_System>() << ")" << std::endl;
				return;
			}
			//(*mask)[SystemsTypes::getTypeIndex<_System>()] = false;

			
			std::get<SystemInfo<_System>>(systems_info).unlinkEntity(entity_id);
		}

	private:
		template <typename ..._Systems>
		void run_systems_preinits(Utils::TypesPack<_Systems...>) { (run_system_preinit<_Systems>(), ...); }
		template<typename _SystemPtr>
		void run_system_preinit()
		{
			if constexpr (SystemInfo<_SystemPtr>::init_methods_count != 0)
			{
				auto &system_info = std::get<SystemInfo<_SystemPtr>>(systems_info);

				system_info.passed_inits = 0;
				for (const auto entity_id : system_info.entities_queues[SystemInfo::TO_INIT])
					system_info.entities_queues[SystemInfo::INITING].push_back(entity_id);
				system_info.entities_queues[SystemInfo::TO_INIT].clear();
			}
		}
		template<typename ..._Orders>
		void run_inits_orders(Utils::TypesPack<_Orders...>) { (run_systems_inits<_Orders>(SystemsTypes{}), ...); }
		template <typename _Order, typename ..._Systems>
		void run_systems_inits(Utils::TypesPack<_Systems...>) { (run_system_init<_Systems, _Order>(), ...); }
		template<typename _System, typename _Order>
		void run_system_init()
		{
			if constexpr (has_init<_System, _Order>::value)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);

				for (const auto entity_id : system_info.entities_queues[SystemInfo::INITING])
					if (entity_id != ECS::EntityIdType_Invalid)
						std::get<_System*>(systems)->init(_Order{}, entity_id);
				++system_info.passed_inits;

				if (SystemInfo<_System>::init_methods_count == system_info.passed_inits)
				{
					for (const auto entity_id : system_info.entities_queues[SystemInfo::INITING])
						system_info.entities_queues[SystemInfo::UPDATE].push_back(entity_id);
					system_info.entities_queues[SystemInfo::INITING].clear();
				}
			}
		}

		template <typename ..._Systems>
		void run_systems_predestroy(Utils::TypesPack<_Systems...>) { (run_system_predestroy<_Systems>(), ...); }
		template<typename _SystemPtr>
		void run_system_predestroy()
		{
			auto &system_info = std::get<SystemInfo<_SystemPtr>>(systems_info);
			if constexpr (SystemInfo<_SystemPtr>::destroy_methods_count != 0)
			{
				system_info.passed_destroys = 0;
				for (const auto entity_id : system_info.entities_queues[SystemInfo::TO_DESTROY])
					system_info.entities_queues[SystemInfo::DESTROYING].push_back(entity_id);
			}
			system_info.entities_queues[SystemInfo::TO_DESTROY].clear();
		}
		template<typename ..._Orders>
		void run_destroys_orders(Utils::TypesPack<_Orders...>) { (run_systems_destroys<_Orders>(SystemsTypes{}), ...); }
		template <typename _Order, typename ..._Systems>
		void run_systems_destroys(Utils::TypesPack<_Systems...>) { (run_system_destroy<_Systems, _Order>(), ...); }
		template<typename _System, typename _Order>
		void run_system_destroy()
		{
			if constexpr (has_destroy<_System, _Order>::value)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);

				for (const auto entity_id : system_info.entities_queues[SystemInfo::INITING])
					if (entity_id != ECS::EntityIdType_Invalid)
						std::get<_System*>(systems)->destroy(_Order{}, entity_id);
				++system_info.passed_destroys;

				if (SystemInfo<_System>::destroy_methods_count == system_info.passed_destroys)
				{
					for (const auto entity_id : system_info.entities_queues[SystemInfo::INITING])
						system_info.entities_queues[SystemInfo::UPDATE].push_back(entity_id);
					system_info.entities_queues[SystemInfo::INITING].clear();
				}
			}
		}

		void construct_systems() { construct_systems_impl(SystemsTypes{}); }
		template<typename ..._Systems>
		void construct_systems_impl(Utils::TypesPack<_Systems...>) { (construct_system<_Systems>(), ...); }
		template<typename _System>
		void construct_system() { std::get<_System*>(systems) = new _System(*this); }

		void destruct_systems() { destruct_systems_impl(SystemsTypes{}); }
		template<typename ..._Systems> 
		void destruct_systems_impl(Utils::TypesPack<_Systems...>) { (destruct_system<_Systems>(), ...); }
		template<typename _System>
		void destruct_system() { delete std::get<_System*>(systems); }

	private:
		ECS::EntityManager entity_manager;
		Utils::ChunkTable<std::bitset<SystemsTypes::types_count>> entity_systems_masks;
		ComponentManagerType component_manager;
		SystemsCollection systems;
		SystemsInfoCollection systems_info;

		bool is_needed_to_stop = false;
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


