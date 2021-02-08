#pragma once

#include "ecs/ECS.h"

// WARNING: Don't use any VIRTUAL functional!
class BaseSystem
{
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
};