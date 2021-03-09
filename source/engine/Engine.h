#pragma once

#include <iostream>
#include <type_traits>

#include "ecs/ECS.h"
#include "Components.h"
#include "SystemsOrders.h"
#include "Systems.h"

namespace Common
{
	class Engine
	{
	private:
		using ComponentManagerType = decltype(Utils::convertTypesPack<ECS::ComponentManager>(ComponentsTypes{}));
		using SystemsCollection = decltype(Utils::convertTypesPack<std::tuple>(convertTypesToPointersPack(SystemsTypes{})));

		struct MethodType { enum { NONE, COMMON, ENTITY }; };
		#define DECLARE_METHOD_CHECKER(METHOD_NAME)\
		template<typename _System, typename _OrderType>\
		struct has_##METHOD_NAME {\
			template<typename _Type, void(_Type::*)(_OrderType)> struct func_pattern_common {};\
			template<typename _Type> static constexpr std::integral_constant<int, MethodType::COMMON> check_func(func_pattern_common<_Type, &_Type::METHOD_NAME>*);\
			template<typename _Type, void(_Type::*)(_OrderType, const ECS::EntityIdType)> struct func_pattern_entity {};\
			template<typename _Type> static constexpr std::integral_constant<int, MethodType::ENTITY> check_func(func_pattern_entity<_Type, &_Type::METHOD_NAME>*);\
			template<typename _Type> static constexpr std::integral_constant<int, MethodType::NONE> check_func(...);\
			static constexpr auto value_common = decltype(check_func<_System>(nullptr))::value == MethodType::COMMON;\
			static constexpr auto value_entity = decltype(check_func<_System>(nullptr))::value == MethodType::ENTITY; };
		DECLARE_METHOD_CHECKER(init);
		DECLARE_METHOD_CHECKER(update);
		DECLARE_METHOD_CHECKER(postUpdate);
		DECLARE_METHOD_CHECKER(destroy);
		#undef DECLARE_METHOD_CHECKER

	private:
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
			static constexpr size_t check_inits(Utils::TypesPack<_Orders...>) { return (has_init<_System, _Orders>::value_entity + ...); }
			static constexpr auto init_methods_number = check_inits(Common::SystemsOrders::Init{});

			template<typename ..._Orders>
			static constexpr size_t check_destroys(Utils::TypesPack<_Orders...>) { return (has_destroy<_System, _Orders>::value_entity + ...); }
			static constexpr auto destroy_methods_number = check_destroys(Common::SystemsOrders::Destroy{});


			std::vector<ECS::EntityIdType> entities_queues[EntitySystemState::NUMBER];
			Utils::ChunkTable<uint8_t> entity_states;

			bool common_is_inited = false;

			_System system;
		};
		using SystemsInfoCollection = SystemsTypes::WrappedPack<std::tuple, SystemInfo>::type;
		template<typename ..._Systems>
		SystemsInfoCollection create_SystemsInfoCollection(Utils::TypesPack<_Systems...>) { return SystemsInfoCollection(SystemInfo<_Systems>(*this)...); }

	public:
		void run()
		{
			engine_state = EngineState::RUN;

			while (true)
			{
				flush_systems_inits(SystemsTypes{});
				run_inits_orders(SystemsOrders::Init{});

				flush_systems_updates(SystemsTypes{});
				run_update_orders(SystemsOrders::Update{});
				run_postUpdate_orders(SystemsOrders::PostUpdate{});
				cleanup_systems_updates(SystemsTypes{});

				flush_systems_destroys(SystemsTypes{});
				flush_entities_remove_queue();
				run_destroys_orders(SystemsOrders::Destroy{});
				cleanup_systems_destroys(SystemsTypes{});
				remove_entities_queue();

				if (engine_state == EngineState::STOP)
				{
					const auto entities_ids = entity_manager.getEntities();
					for (const auto e_id : entities_ids) removeEnity(e_id);

					flush_systems_destroys(SystemsTypes{});
					flush_entities_remove_queue();

					run_destroys_orders<true>(SystemsOrders::Destroy{});
					remove_entities_queue();

					return;
				}
			}
		}

		void stop() { engine_state = EngineState::STOP; }

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
		auto &getSystem() { return std::get<SystemInfo<_System>>(systems_info).system; }
		
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
				 std::get<SystemInfo<_System>>(systems_info).switchEntitiesState(EntitySystemState::TO_INIT, EntitySystemState::INITING);
		}
		template<typename ..._Orders>
		void run_inits_orders(Utils::TypesPack<_Orders...>) { (run_systems_inits<_Orders>(SystemsTypes{}), ...); }
		template <typename _Order, typename ..._Systems>
		void run_systems_inits(Utils::TypesPack<_Systems...>) { (run_system_init<_Systems, _Order>(), ...); }
		template<typename _System, typename _Order>
		void run_system_init()
		{
			if constexpr (has_init<_System, _Order>::value_common)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);
				if (!system_info.common_is_inited)
				{
					system_info.system.init(_Order{});
					system_info.common_is_inited = true;
				}
			}
			if constexpr (has_init<_System, _Order>::value_entity)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);

				for (const auto entity_id : system_info.entities_queues[EntitySystemState::INITING])
					if (entity_id != ECS::EntityIdType_Invalid)
						system_info.system.init(_Order{}, entity_id);
			}
		}

		template <typename ..._Systems>
		void flush_systems_updates(Utils::TypesPack<_Systems...>) { (flush_system_update<_Systems>(), ...); }
		template<typename _System>
		void flush_system_update() { std::get<SystemInfo<_System>>(systems_info).switchEntitiesState(EntitySystemState::INITING, EntitySystemState::UPDATE); }
		template<typename ..._Orders>
		void run_update_orders(Utils::TypesPack<_Orders...>) { (run_systems_updates<_Orders>(SystemsTypes{}), ...); }
		template <typename _Order, typename ..._Systems>
		void run_systems_updates(Utils::TypesPack<_Systems...>) { (run_system_update<_Systems, _Order>(), ...); }
		template<typename _System, typename _Order>
		void run_system_update()
		{
			if constexpr (has_update<_System, _Order>::value_common)
			{
				if (engine_state == EngineState::RUN)
					std::get<SystemInfo<_System>>(systems_info).system.update(_Order{});
			}
			if constexpr (has_update<_System, _Order>::value_entity)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);

				for (const auto entity_id : system_info.entities_queues[EntitySystemState::UPDATE])
				{
					if (engine_state != EngineState::RUN)
						break;
					if (entity_id != ECS::EntityIdType_Invalid)
						system_info.system.update(_Order{}, entity_id);
				}
			}
		}
		template<typename ..._Orders>
		void run_postUpdate_orders(Utils::TypesPack<_Orders...>) { (run_systems_postUpdates<_Orders>(SystemsTypes{}), ...); }
		template <typename _Order, typename ..._Systems>
		void run_systems_postUpdates(Utils::TypesPack<_Systems...>) { (run_system_postUpdate<_Systems, _Order>(), ...); }
		template<typename _System, typename _Order>
		void run_system_postUpdate()
		{
			if constexpr (has_postUpdate<_System, _Order>::value_common)
			{
				if (engine_state == EngineState::RUN)
					std::get<SystemInfo<_System>>(systems_info).system.postUpdate(_Order{});
			}
			if constexpr (has_postUpdate<_System, _Order>::value_entity)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);

				for (const auto entity_id : system_info.entities_queues[EntitySystemState::UPDATE])
				{
					if (engine_state != EngineState::RUN)
						break;
					if (entity_id != ECS::EntityIdType_Invalid)
						system_info.system.postUpdate(_Order{}, entity_id);
				}
			}
		}
		template <typename ..._Systems>
		void cleanup_systems_updates(Utils::TypesPack<_Systems...>) { (flush_system_update<_Systems>(), ...); }
		template<typename _System>
		void cleanup_system_update()
		{
			if (auto &entities = std::get<SystemInfo<_System>>(systems_info).entities_queues[EntitySystemState::UPDATE]; entities.size())
			{
				auto it = entities.data();
				for (auto swap_it = entities.data() + entities.size() - 1; it <= swap_it;)
				{
					if (*it != ECS::EntityIdType_Invalid)
					{
						++it;
						continue;
					}
					if (*swap_it != ECS::EntityIdType_Invalid)
					{
						std::swap(*it, *swap_it);
						++it;
					}
					--swap_it;
				}
				entities.resize(it - entities.data());
			}
		}

		template <typename ..._Systems>
		void flush_systems_destroys(Utils::TypesPack<_Systems...>) { (flush_system_destroy<_Systems>(), ...); }
		template<typename _System>
		void flush_system_destroy()
		{
			if constexpr (SystemInfo<_System>::destroy_methods_number != 0)
				std::get<SystemInfo<_System>>(systems_info).switchEntitiesState(EntitySystemState::TO_DESTROY, EntitySystemState::DESTROYING);
		}
		template<bool _is_run_common = false, typename ..._Orders>
		void run_destroys_orders(Utils::TypesPack<_Orders...>) { (run_systems_destroys<_is_run_common, _Orders>(SystemsTypes{}), ...); }
		template <bool _is_run_common, typename _Order, typename ..._Systems>
		void run_systems_destroys(Utils::TypesPack<_Systems...>) { (run_system_destroy<_is_run_common, _Systems, _Order>(), ...); }
		template<bool _is_run_common, typename _System, typename _Order>
		void run_system_destroy()
		{
			if constexpr (has_destroy<_System, _Order>::value_common && _is_run_common)
			{
				std::get<SystemInfo<_System>>(systems_info).system.destroy(_Order{});
			}
			if constexpr (has_destroy<_System, _Order>::value_entity)
			{
				auto &system_info = std::get<SystemInfo<_System>>(systems_info);
				for (const auto entity_id : system_info.entities_queues[EntitySystemState::DESTROYING])
					if (entity_id != ECS::EntityIdType_Invalid)
						system_info.system.destroy(_Order{}, entity_id);
			}
		}
		template <typename ..._Systems>
		void cleanup_systems_destroys(Utils::TypesPack<_Systems...>) { (flush_system_update<_Systems>(), ...); }
		template<typename _System>
		void cleanup_system_destroy()
		{
			auto &system_info = std::get<SystemInfo<_System>>(systems_info);
			for (const auto entity_id : system_info.entities_queues[EntitySystemState::DESTROYING])
			{
				system_info.entity_states.remove(entity_id);
				(*entity_systems_masks.get(entity_id))[SystemsTypes::getTypeIndex<_System>()] = false;
			}
			system_info.entities_queues[EntitySystemState::DESTROYING].clear();
		}

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
		SystemsInfoCollection systems_info{ create_SystemsInfoCollection(SystemsTypes{}) };

		struct EngineState
		{
			enum
			{
				STOP
				, RUN
			};
		};
		uint8_t engine_state = EngineState::STOP;
	};
}