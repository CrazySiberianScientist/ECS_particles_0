#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/gl.h>
#include <gl/GL.h>

#include <iostream>

#include "ecs/ECS.h"
#include "Components.h"
#include "SystemsOrders.h"
#include "Systems.h"

struct GLFWwindow;

namespace Common
{
	class Engine
	{
	public:
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

	public:
		struct EntityRemoveState
		{
			enum
			{
				TO_REMOVE
				, REMOVING
				, NUMBER
			};
		};

		struct EntitySystemState
		{
			enum
			{
				TO_INIT
				, INITING
				, UPDATE
				, TO_DESTROY
				, DESTROYING
				, NUMBER
			};
		};

		template<typename _System>
		struct SystemInfo
		{
			SystemInfo(Engine &engine) : system(engine) {}

			void linkEntity(const ECS::EntityIdType entity_id)
			{
				if constexpr (SystemInfo<_System>::init_methods_number != 0)
				{
					entities_queues[EntitySystemState::TO_INIT].push_back(entity_id);
					*(entity_states.emplace(entity_id)) = EntitySystemState::TO_INIT;
				}
				else
				{
					entities_queues[EntitySystemState::UPDATE].push_back(entity_id);
					*(entity_states.emplace(entity_id)) = EntitySystemState::UPDATE;
				}
			}

			void unlinkEntity(const ECS::EntityIdType entity_id)
			{
				auto &state = *(entity_states.get(entity_id));

				if (state == EntitySystemState::TO_DESTROY || state == EntitySystemState::DESTROYING)
				{
					std::cerr << "[Warning] " << __FUNCTION__ << " - Entity(ID " << entity_id << ") that is linked to System(Index " 
						<< SystemsTypes::getTypeIndex<_System>() << ") already in Unlinking process" << std::endl;
					return;
				}
				(*std::find(entities_queues[state].begin(), entities_queues[state].end(), entity_id)) = ECS::EntityIdType_Invalid;
				

				if constexpr (SystemInfo<_System>::destroy_methods_number != 0)
				{
					state = EntitySystemState::TO_DESTROY;
					entities_queues[EntitySystemState::TO_DESTROY].push_back(entity_id);
				}
				else entity_states.remove(entity_id);
			}

			void switchEntitiesState(const uint8_t from_state, const uint8_t to_state)
			{
				for (const auto entity_id : entities_queues[from_state])
					if (entity_id != ECS::EntityIdType_Invalid)
					{
						entities_queues[to_state].push_back(entity_id);
						*entity_states.get(entity_id) = to_state;
					}
				entities_queues[from_state].clear();
			}


			template<typename ..._Orders>
			static constexpr size_t check_inits(Utils::TypesPack<_Orders...>) { return (has_init<_System, _Orders>::value + ...); }
			static constexpr auto init_methods_number = check_inits(Common::SystemsOrders::Init{});
			template<typename ..._Orders>
			static constexpr size_t check_destroys(Utils::TypesPack<_Orders...>) { return (has_destroy<_System, _Orders>::value + ...); }
			static constexpr auto destroy_methods_number = check_destroys(Common::SystemsOrders::Destroy{});

			std::vector<ECS::EntityIdType> entities_queues[EntitySystemState::NUMBER];
			Utils::ChunkTable<uint8_t> entity_states;

			uint32_t passed_inits = 0;
			uint32_t passed_destroys = 0;

			_System system;
		};
		using SystemsInfoCollection = SystemsTypes::WrappedPack<std::tuple, SystemInfo>::type;
		template<typename ..._Systems>
		SystemsInfoCollection create_SystemsInfoCollection(Utils::TypesPack<_Systems...>) { return SystemsInfoCollection(SystemInfo<_Systems>(*this)...); }

	public:
		Engine()
		{
			construct_systems();
		}

		~Engine()
		{
			destruct_systems();
		}

		void run()
		{
			while (true)
			{
				flush_systems_inits(SystemsTypes{});
				flush_systems_destroys(SystemsTypes{});
				flush_entities_remove_queue();

				run_inits_orders(SystemsOrders::Init{});
				run_destroys_orders(SystemsOrders::Destroy{});

				remove_entities_queue();

				//if (is_needed_to_stop)
				{
					const auto entities_ids = entity_manager.getEntities();
					for (const auto e_id : entities_ids) removeEnity(e_id);

					flush_systems_destroys(SystemsTypes{});
					flush_entities_remove_queue();

					run_destroys_orders(SystemsOrders::Destroy{});
					remove_entities_queue();

					return;
				}
			}
		}

		/* TODO: I thought about realization via exception, but I'm not sure.
		There is needed way to immediately engine stop.*/
		void stop()
		{
			is_needed_to_stop = true;
		}

		auto createEntity()
		{
			const auto entity_id = entity_manager.create();
			if (entity_id == ECS::EntityIdType_Invalid) std::cerr << "[Warning] " << __FUNCTION__ << " - Cannot create new Entity, maximum count" << std::endl;
			else entity_systems_masks.emplace(entity_id);
			return entity_id;
		}
		void removeEnity(const ECS::EntityIdType entity_id)
		{
			if (entity_id == ECS::EntityIdType_Invalid)
			{
				std::cerr << "[Warning] " << __FUNCTION__ << " - entity_id is invalid" << std::endl;
				return;
			}

			entities_remove_queue[EntityRemoveState::TO_REMOVE].push_back(entity_id);
			unlinkEntityFromAllSystems(entity_id);
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
				std::cerr << "[Warning] " << __FUNCTION__ << " - Entity(ID " << entity_id << ") isn't exsist" << std::endl;
				return;
			}
			if ((*mask)[SystemsTypes::getTypeIndex<_System>()])
			{
				std::cerr << "[Warning] " << __FUNCTION__ << " - Entity(ID " << entity_id << ") is already linked to System(Index " 
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
				std::cerr << "[Warning] " << __FUNCTION__ << " - Entity(ID " << entity_id << ") isn't exsist" << std::endl;
				return;
			}
			if (!(*mask)[SystemsTypes::getTypeIndex<_System>()])
				return;

			if constexpr (SystemInfo<_System>::destroy_methods_number == 0)
				(*mask)[SystemsTypes::getTypeIndex<_System>()] = false;
			
			std::get<SystemInfo<_System>>(systems_info).unlinkEntity(entity_id);
		}
		
		void unlinkEntityFromAllSystems(const ECS::EntityIdType entity_id)
		{
			unlinkEntityFromAllSystems_impl(SystemsTypes{}, entity_id);
		}

	private:
		template<typename ..._Systems>
		void unlinkEntityFromAllSystems_impl(Utils::TypesPack<_Systems...>, const ECS::EntityIdType entity_id)
		{
			(unlinkEntityFromSystem<_Systems>(entity_id), ...);
		}

		template <typename ..._Systems>
		void flush_systems_inits(Utils::TypesPack<_Systems...>) { (flush_system_init<_Systems>(), ...); }
		template<typename _System>
		void flush_system_init()
		{
			if constexpr (SystemInfo<_System>::init_methods_number != 0)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);

				system_info.passed_inits = 0;
				system_info.switchEntitiesState(EntitySystemState::TO_INIT, EntitySystemState::INITING);
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

				for (const auto entity_id : system_info.entities_queues[EntitySystemState::INITING])
					if (entity_id != ECS::EntityIdType_Invalid)
						std::get<_System*>(systems)->init(_Order{}, entity_id);
				++system_info.passed_inits;

				if (SystemInfo<_System>::init_methods_number == system_info.passed_inits)
					system_info.switchEntitiesState(EntitySystemState::INITING, EntitySystemState::UPDATE);
			}
		}

		template <typename ..._Systems>
		void flush_systems_destroys(Utils::TypesPack<_Systems...>) { (flush_system_destroy<_Systems>(), ...); }
		template<typename _System>
		void flush_system_destroy()
		{
			if constexpr (SystemInfo<_System>::destroy_methods_number != 0)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);

				system_info.passed_inits = 0;
				system_info.switchEntitiesState(EntitySystemState::TO_DESTROY, EntitySystemState::DESTROYING);
			}
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

				for (const auto entity_id : system_info.entities_queues[EntitySystemState::DESTROYING])
					if (entity_id != ECS::EntityIdType_Invalid)
						std::get<_System*>(systems)->destroy(_Order{}, entity_id);
				++system_info.passed_destroys;

				if (SystemInfo<_System>::destroy_methods_number == system_info.passed_destroys)
				{
					for (const auto entity_id : system_info.entities_queues[EntitySystemState::DESTROYING])
					{
						system_info.entity_states.remove(entity_id);
						(*entity_systems_masks.get(entity_id))[SystemsTypes::getTypeIndex<_System>()] = false;
					}
					system_info.entities_queues[EntitySystemState::DESTROYING].clear();
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

		void flush_entities_remove_queue()
		{
			for (const auto entity_id : entities_remove_queue[EntityRemoveState::TO_REMOVE])
				entities_remove_queue[EntityRemoveState::REMOVING].push_back(entity_id);
			entities_remove_queue[EntityRemoveState::TO_REMOVE].clear();
		}
		void remove_entities_queue()
		{
			for (const auto entity_id : entities_remove_queue[EntityRemoveState::REMOVING])
			{
				component_manager.removeAllComponents(entity_id);
				entity_manager.remove(entity_id);
				entity_systems_masks.remove(entity_id);
			}
			entities_remove_queue[EntityRemoveState::REMOVING].clear();
		}

	private:
		ECS::EntityManager entity_manager;
		Utils::ChunkTable<std::bitset<SystemsTypes::types_count>> entity_systems_masks;
		std::vector<ECS::EntityIdType> entities_remove_queue[EntityRemoveState::NUMBER];
		ComponentManagerType component_manager;
		SystemsCollection systems;
		SystemsInfoCollection systems_info = create_SystemsInfoCollection(SystemsTypes{});

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


