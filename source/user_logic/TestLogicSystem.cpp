#include "TestLogicSystem.h"
#include "engine/Engine.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
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
		Utils::StructOfVectors<glm::vec3, glm::quat, glm::vec3> transforms_buffer;
		const auto &system_entities = engine.getSystemEntities<TestLogicSystem>();
		for (const auto e : system_entities)
		{
			if (e == ECS::EntityIdType_Invalid) continue;
			auto t = engine.getComponentManager().getComponent<EngineLogic::Components::Transform>(e);
			if (!t) continue;
			transforms_buffer.append(t->pos, t->rot, t->scale);
		}

		{
			GLuint buffer_id;
			glGenBuffers(1, &buffer_id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_id);
			glBufferData(GL_SHADER_STORAGE_BUFFER
				, sizeof(glm::vec3) * transforms_buffer.size()
				, const_cast<glm::vec3*>(transforms_buffer.data<0>())
				, GL_STATIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, buffer_id);
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
		glDrawArrays(GL_TRIANGLES, 0, transforms_buffer.size() * 6);
	}
}
