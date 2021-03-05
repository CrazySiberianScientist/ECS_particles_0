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
	
	private:
		GLuint vertex_buffer, program;
		GLint mvp_location, vpos_location, vcol_location;

		struct Vertex
		{
			vec3 pos;
			vec3 color;
		};

		inline static const Vertex triangle_vertices[3]{
			{ -0.6f, -0.4f, 0.0f, 1.f, 0.f, 0.f },
			{  0.6f, -0.4f, 0.0f, 0.f, 1.f, 0.f },
			{   0.f,  0.6f, 0.0f, 0.f, 0.f, 1.f } };
	};

}