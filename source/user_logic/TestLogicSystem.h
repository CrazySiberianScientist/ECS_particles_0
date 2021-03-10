#pragma once

#include "engine/BaseSystem.h"
#include "UserSystemsOrders.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace UserLogic
{
	class TestLogicSystem : public BaseSystem
	{
	public:
		TestLogicSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void init(SystemsOrders::Init::TEST_0);
		void update(SystemsOrders::Update::TEST_PARTICLES);
		void update(SystemsOrders::Update::TEST_PARTICLES_MOVE, const ECS::EntityIdType entity_id);
	
	private:
		GLuint program;
		GLint vp_transform_location;
	};

}