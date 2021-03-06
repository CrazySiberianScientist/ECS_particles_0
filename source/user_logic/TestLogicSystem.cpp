#include "TestLogicSystem.h"
#include "engine/Engine.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace UserLogic
{
	void TestLogicSystem::init(SystemsOrders::Init::TEST_0)
	{
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

		const auto vertex_shader = engine.getSystem<EngineLogic::ShadersSystem>().getShaderId(u8"triangle_test.vert");
		const auto fragment_shader = engine.getSystem<EngineLogic::ShadersSystem>().getShaderId(u8"triangle_test.frag");
		if (vertex_shader == 0 || fragment_shader == 0) return;

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		mvp_location = glGetUniformLocation(program, "MVP");
		vpos_location = glGetAttribLocation(program, "vPos");
		vcol_location = glGetAttribLocation(program, "vCol");

		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(triangle_vertices[0]), (void*)0);
		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(triangle_vertices[0]), (void*)(sizeof(Vertex::pos)));
	}

	void TestLogicSystem::update(SystemsOrders::Update::TEST_0)
	{
		float ratio;
		int width, height;
		mat4x4 m, p, mvp;

		glfwGetFramebufferSize(engine.getSystem<EngineLogic::AppSystem>().getGLFWwindow(), &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);


		const auto rot_mat = glm::rotate(glm::mat4(), (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void TestLogicSystem::update(SystemsOrders::Update::TEST_TRIANGLE, const ECS::EntityIdType entity_id)
	{
		auto transform = engine.getComponentManager().getComponent<EngineLogic::Components::Transform>(entity_id);
	}

}
