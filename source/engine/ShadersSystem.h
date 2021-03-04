#pragma once

#include "BaseSystem.h"
#include "EngineSystemsOrders.h"

#include <unordered_map>
#include <filesystem>
#include <glad/gl.h>

namespace EngineLogic
{
	class ShadersSystem : public BaseSystem
	{
	public:
		ShadersSystem(Common::Engine &engine_) : BaseSystem(engine_) {}

		void init(SystemsOrders::Init::SHADERS);

		GLuint getShaderId(const std::string &shader_name);

	private:
		std::string shaders_dir = u8"../data/shaders/";
		std::unordered_map<std::string, GLuint> shaders;
		inline static const std::unordered_map<std::string, GLuint> shaders_extensions {
			{u8".frag", GL_FRAGMENT_SHADER}
			, {u8".vert", GL_VERTEX_SHADER} };
	};
}