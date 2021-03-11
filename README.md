# Entity Component System (ECS) Demo
![title image](https://github.com/CrazySiberianScientist/ECS_particles_0/blob/master/docs/title_image.gif)

## Introduction

Hello, comrades! This is my training project, it consists of two parts : **ECS** and **OpenGL based Dummy-Engine**.  
Main purpose of this project - create ECS with minimal perfomance overhead *(ha-ha-ha)*, because of this I preferred static polymorphism (template magic), flat memory collections and hierarchies avoidance.  
Because this project is training there are some disadvantages:
* Project isn't well tested
* I tried to implement cross-platform code, but it isn't configured for Linux build
* There are may be strange and weird names and architecture decisions
* Isn't thread safe


## Requirements and external dependencies

ECS part:
* C++17 compiler (repository includes MS Visual Studio 2019 project)  

Dummy-Engine part:
* GLFW (https://www.glfw.org/). In project global variable **GLFW_DIR** is used.
* GLM (https://github.com/g-truc/glm). In project global variable **GLM_DIR** is used.
* OpenGL extensions (https://www.khronos.org/registry/OpenGL/index_gl.php). In project global variable **OPENGL_EXT_DIR** is used.


## Entity Component System (ECS) part

Necessary files:
* *./ecs/ECS.h*
* *./utils/Utils.h*  

Important classes:  
* **EntityManager** - factory of entities, return Entity ID (***EntityIdType***)
* **ComponentCollection**<***_Type***> - components factory-collection of ***_Type***, contains ***Utils::ChunkBuffer*** and ***Utils::ChunkTable***  
* **ComponentManager**<***_Types***> - aggregates a lot of **ComponentCollection** from ***_Types***


## Dummy Engine part

Necessary files: *all projects files are neccessary*

Important classes:
* **BaseSystem** and it's Inheritances  
	All **Systems** must be inherited from **BaseSystem**.  
	There are two kinds of systems: ***Engine*** (*./engine* folder) and ***User*** (*./user_logic* folder).  
	**System** can have next methods:  
 	```cpp
	// Methods per entity
	void init(SystemsOrders::Init::_ORDER_, const ECS::EntityIdType entity_id);
 	void update(SystemsOrders::Update::_ORDER_, const ECS::EntityIdType entity_id);
 	void postUpdate(SystemsOrders::PostUpdate::_ORDER_, const ECS::EntityIdType entity_id);
 	void destroy(SystemsOrders::Destroy::_ORDER_, const ECS::EntityIdType entity_id);
	void init(SystemsOrders::Init::_ORDER_);
	// Common methods, called once per frame (except init(...) and destroy(...)
	void init(SystemsOrders::Init::_ORDER_);
 	void update(SystemsOrders::Update::_ORDER_);
 	void postUpdate(SystemsOrders::PostUpdate::_ORDER_);
 	void destroy(SystemsOrders::Destroy::_ORDER_);
	```  

* **Common::Engine** (*engine/Engine.h*)  
Aggregates **EntityManager**, **ComponentManager**, and a lot of **Systems**. Calls methods of **Systems** via next order (see table). **Init** - methods call once, when **entity** is just linked to System, but isn't immidiately, entity pushed to init-queue and inited on next frame. **Destoy** - methods call once, when **entity** is just unlinked to System, but isn't immidiately, entity pushed to destroy-queue and destroyed on next frame.

Systems methods ordrers |
----------------------- |
**Engine init :** |
SomeEngineSystem::init(EngineLogic::SystemsOrders::Init::ORDER_0) |
SomeEngineSystem::init(EngineLogic::SystemsOrders::Init::ORDER_N...) |
SomeEngineSystem::init(EngineLogic::SystemsOrders::Init::ORDER_0, const ECS::EntityIdType) |
SomeEngineSystem::init(EngineLogic::SystemsOrders::Init::ORDER_N..., const ECS::EntityIdType) |
**User init :** |
SomeUserSystem::init(UserLogic::SystemsOrders::Init::ORDER_0) |
SomeUserSystem::init(UserLogic::SystemsOrders::Init::ORDER_N...) |
SomeUserSystem::init(UserLogic::SystemsOrders::Init::ORDER_0, const ECS::EntityIdType) |
SomeUserSystem::init(UserLogic::SystemsOrders::Init::ORDER_N..., const ECS::EntityIdType) |
**System update :** |
SomeEngineSystem::update(EngineLogic::SystemsOrders::update::ORDER_0) |
SomeEngineSystem::update(EngineLogic::SystemsOrders::update::ORDER_N...) |
SomeEngineSystem::update(EngineLogic::SystemsOrders::update::ORDER_0, const ECS::EntityIdType) |
SomeEngineSystem::update(EngineLogic::SystemsOrders::update::ORDER_N..., const ECS::EntityIdType) |
**User update :** |
SomeUserSystem::update(UserLogic::SystemsOrders::update::ORDER_0) |
SomeUserSystem::update(UserLogic::SystemsOrders::update::ORDER_N...) |
SomeUserSystem::update(UserLogic::SystemsOrders::update::ORDER_0, const ECS::EntityIdType) |
SomeUserSystem::update(UserLogic::SystemsOrders::update::ORDER_N..., const ECS::EntityIdType) |
**User postUpdate :** |
SomeUserSystem::postUpdate(UserLogic::SystemsOrders::postUpdate::ORDER_0) |
SomeUserSystem::postUpdate(UserLogic::SystemsOrders::postUpdate::ORDER_N...) |
SomeUserSystem::postUpdate(UserLogic::SystemsOrders::postUpdate::ORDER_0, const ECS::EntityIdType) |
SomeUserSystem::postUpdate(UserLogic::SystemsOrders::postUpdate::ORDER_N..., const ECS::EntityIdType) |
**System postUpdate :** |
SomeEngineSystem::postUpdate(EngineLogic::SystemsOrders::postUpdate::ORDER_0) |
SomeEngineSystem::postUpdate(EngineLogic::SystemsOrders::postUpdate::ORDER_N...) |
SomeEngineSystem::postUpdate(EngineLogic::SystemsOrders::postUpdate::ORDER_0, const ECS::EntityIdType) |
SomeEngineSystem::postUpdate(EngineLogic::SystemsOrders::postUpdate::ORDER_N..., const ECS::EntityIdType) |
**User destroy :** |
SomeUserSystem::destroy(UserLogic::SystemsOrders::destroy::ORDER_0) |
SomeUserSystem::destroy(UserLogic::SystemsOrders::destroy::ORDER_N...) |
SomeUserSystem::destroy(UserLogic::SystemsOrders::destroy::ORDER_0, const ECS::EntityIdType) |
SomeUserSystem::destroy(UserLogic::SystemsOrders::destroy::ORDER_N..., const ECS::EntityIdType) |
**System destroy :** |
SomeEngineSystem::destroy(EngineLogic::SystemsOrders::destroy::ORDER_0) |
SomeEngineSystem::destroy(EngineLogic::SystemsOrders::destroy::ORDER_N...) |
SomeEngineSystem::destroy(EngineLogic::SystemsOrders::destroy::ORDER_0, const ECS::EntityIdType) |
SomeEngineSystem::destroy(EngineLogic::SystemsOrders::destroy::ORDER_N..., const ECS::EntityIdType) |

* Each methods order must be registered in ***EngineSystemsOrders.h*** or ***UserSystemsOrders.h***
* Each component must be registered in ***EngineComponents.h*** or ***UserComponents.h***
* Each system must be registered in ***EngineSystems.h*** or ***UserSystems.h***