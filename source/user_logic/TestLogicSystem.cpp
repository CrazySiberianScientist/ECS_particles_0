#include "TestLogicSystem.h"
#include "engine/Engine.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <GL/glext.h>

#include "utils/StructOfVectors.h"

namespace UserLogic
{
	void TestLogicSystem::init(SystemsOrders::Init::TEST_0)
	{
		const auto vertex_shader = engine.getSystem<EngineLogic::ShadersSystem>().getShaderId(u8"particles_test.vert");
		const auto fragment_shader = engine.getSystem<EngineLogic::ShadersSystem>().getShaderId(u8"particles_test.frag");
		if (vertex_shader == 0 || fragment_shader == 0) return;

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		vp_transform_location = glGetUniformLocation(program, "vp_transform");
	}

	void TestLogicSystem::update(SystemsOrders::Update::TEST_PARTICLES)
	{
		Utils::StructOfVectors<glm::vec4, glm::quat> transforms_buffer;
		const auto &system_entities = engine.getSystemEntities<TestLogicSystem>();
		
		for (const auto e : system_entities)
		{
			if (e == ECS::EntityIdType_Invalid) continue;
			auto t = engine.getComponentManager().getComponent<EngineLogic::Components::Transform>(e);
			if (!t) continue;
			transforms_buffer.append(glm::vec4(t->pos, 1.0f), t->rot);
		}

		{
			GLuint buffer_id;
			const GLuint buffer_bind_id = 0;
			glGenBuffers(1, &buffer_id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_id);
			glBufferData(GL_SHADER_STORAGE_BUFFER
				, sizeof(glm::vec4) * transforms_buffer.size()
				, const_cast<glm::vec4*>(transforms_buffer.data<0>())
				, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer_bind_id, buffer_id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		{
			GLuint buffer_id;
			const GLuint buffer_bind_id = 1;
			glGenBuffers(1, &buffer_id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_id);
			glBufferData(GL_SHADER_STORAGE_BUFFER
				, sizeof(glm::quat) * transforms_buffer.size()
				, const_cast<glm::quat*>(transforms_buffer.data<1>())
				, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer_bind_id, buffer_id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		{
			const auto main_camera = engine.getSystem<EngineLogic::CameraSystem>().getMainCamera();
			if (main_camera == ECS::EntityIdType_Invalid) return;

			const auto * const ct = engine.getComponentManager().getComponent<EngineLogic::Components::CameraTransform>(main_camera);
			const auto vp_mat = ct->projection * ct->view;

			glUniformMatrix4fv(vp_transform_location, 1, GL_FALSE, (const GLfloat*)&vp_mat[0]);
		}

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(transforms_buffer.size() * 6));
	}

	void TestLogicSystem::update(SystemsOrders::Update::TEST_PARTICLES_MOVE, const ECS::EntityIdType entity_id)
	{
		auto t = engine.getComponentManager().getComponent<EngineLogic::Components::Transform>(entity_id);
		if (!t) return;
		auto s = engine.getComponentManager().getComponent<Components::Speed>(entity_id);
		if (!s) return;

		t->pos = t->pos + s->move_vec * engine.getSystem<EngineLogic::AppSystem>().getIFPS();
		if (glm::length2(t->pos) > 1.0f) t->pos = glm::vec3(0.0f);
	}
}
