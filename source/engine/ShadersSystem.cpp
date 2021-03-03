#include "ShadersSystem.h"

#include <iostream>

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
			std::cout << "!!! " << dir.path() << std::endl;

		}
	}
}
