#include "TestLogicSystem.h"
#include "engine/Engine.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <GL/glext.h>

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
		const auto frame_size = engine.getSystem<EngineLogic::AppSystem>().getFrameSize();
		const float ratio = frame_size[0] / (float)frame_size[1];

		const auto main_camera = engine.getSystem<EngineLogic::CameraSystem>().getMainCamera();
		if (main_camera == ECS::EntityIdType_Invalid) return;

		const auto * const ct = engine.getComponentManager().getComponent<EngineLogic::Components::CameraTransform>(main_camera);
		const auto mv_mat = ct->projection * ct->view;

		glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mv_mat[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void TestLogicSystem::update(SystemsOrders::Update::TEST_PARTICLES, const ECS::EntityIdType entity_id)
	{
		GLuint buffer_id;
		glGenBuffers(1, &buffer_id);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_id);

		auto transform = engine.getComponentManager().getComponent<EngineLogic::Components::Transform>(entity_id);
	}
}
