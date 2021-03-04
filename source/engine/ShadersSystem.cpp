#include "ShadersSystem.h"

#include <iostream>
#include <fstream>
#include <string>

namespace EngineLogic
{
	void ShadersSystem::init(SystemsOrders::Init::SHADERS)
	{
		std::filesystem::path shaders_path(shaders_dir);
		if (!std::filesystem::exists(shaders_path))
		{
			std::cerr << "[Error] Shaders path doesn't exist: " << shaders_dir << std::endl;
			return;
		}

		for (const auto &dir : std::filesystem::recursive_directory_iterator(shaders_path))
		{
			if (!dir.is_regular_file()) continue;
			auto found_extension = shaders_extensions.find(dir.path().extension().u8string());
			if (found_extension == shaders_extensions.end()) continue;

			const auto shader_type = found_extension->second;

			std::string text;
			std::ifstream file(dir.path());
			for (std::string line; std::getline(file, line); text.append(line + "\n")) {}

			auto text_ptr = text.c_str();
			const auto shader_id = glCreateShader(shader_type);
			glShaderSource(shader_id, 1, &text_ptr, NULL);
			glCompileShader(shader_id);

			GLint isCompiled = 0;
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				std::cerr << "[Error] Shader compilation failed : " << dir.path() << std::endl;
				continue;
			}

			shaders[dir.path().filename().u8string()] = shader_id;
		}
	}

	GLuint ShadersSystem::getShaderId(const std::string & shader_name)
	{
		auto found_it = shaders.find(shader_name);
		if (found_it == shaders.end())
		{
			std::cerr << "[Error] Can't find shader: " << shader_name << std::endl;
			return GLuint(0);
		}
		return found_it->second;
	}
}
