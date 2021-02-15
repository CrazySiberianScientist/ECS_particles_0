#pragma once

#include "ecs/ECS.h"

namespace Common
{
	class Engine;
}

// WARNING: Don't use any VIRTUAL functional!
class BaseSystem
{
public:
	BaseSystem(Common::Engine &engine_) : engine(engine_) {}

protected:
	Common::Engine &engine;
};

// WARNING: Don't use any VIRTUAL functional!
class GenericSystem : BaseSystem
{
public:
	GenericSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

	void linkEntity(const ECS::EntityIdType entity) { /*some code in inherited classes;*/ link_entity(entity); }
	void unlinkEntity(const ECS::EntityIdType entity) { /*some code in inherited classes;*/ unlink_entity(entity); }

protected:
	void link_entity(const ECS::EntityIdType entity) { entities.emplace_back(entity); }
	void unlink_entity(const ECS::EntityIdType entity)
	{ 
		auto found = std::find(entities.begin(), entities.end(), entity); 
		if (found == entities.end()) return;
		entities.erase(found);
	}

protected:
	std::vector<ECS::EntityIdType> entities;
	std::vector<ECS::EntityIdType> not_inited_entities;
};