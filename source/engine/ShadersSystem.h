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

		auto getShaderId(const std::string &shader_name)
		{
			auto found_it = shaders.find(shader_name);
			if (found_it == shaders.end()) return GLuint(0);
			return found_it->second;
		}

	private:
		std::string shaders_dir = u8"../data/shaders/";
		std::unordered_map<std::string, GLuint> shaders;
		inline static const std::unordered_map<std::string, GLuint> shaders_extensions {
			{u8".frag", GL_FRAGMENT_SHADER}
			, {u8".vert", GL_VERTEX_SHADER} };
	};
}