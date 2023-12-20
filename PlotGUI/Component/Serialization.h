#pragma once

#include <fstream>
#include <filesystem>

#include "ThirdPartys/Json/json.h"

using json = nlohmann::json;

namespace PlotGUI
{
	class Serialization
	{
	public:
		static bool SaveJson(const std::string& path, const std::string& filename, const json& json)
		{
			std::string projPath = PROJET_DIR;

			std::filesystem::create_directories(projPath + path);

			std::ofstream file(projPath + path + filename);

			if (!file)
				return false;

			file << std::setw(4) << json;
			file.close();

			return true;
		}

		static bool ReadJson(const std::string& path, json& json)
		{
			std::string projPath = PROJET_DIR;

			std::ifstream file(projPath + path);

			if (!file)
				return false;

			file >> json;
			file.close();

			return true;
		}
	};
}
