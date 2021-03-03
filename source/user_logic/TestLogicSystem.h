#pragma once

#include "engine/BaseSystem.h"
#include "UserSystemsOrders.h"

#include <glad/gl.h>

namespace UserLogic
{
	class TestLogicSystem : public BaseSystem
	{
	public:
		TestLogicSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void init(SystemsOrders::Init::TEST_0);

		void update(SystemsOrders::Update::TEST_0);
		
		void destroy(SystemsOrders::Destroy::TEST_1, const ECS::EntityIdType e)
		{
			printf("!! %s %s %d\n", __FUNCTION__, "TEST_1", e);
		}
	
	private:
		GLuint vertex_buffer, vertex_shader, fragment_shader, program;
		GLint mvp_location, vpos_location, vcol_location;
	};

}