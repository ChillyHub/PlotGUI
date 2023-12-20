#pragma once

#include <fstream>

#include "ThirdPartys/Json/json.h"

using json = nlohmann::json;

namespace PlotGUI
{
	class Serialization
	{
	public:
		static void SaveJson(const std::string& path, const json& json)
		{
			std::ofstream file(path);
			file << json;
			file.close();
		}

		static json ReadJson(const std::string& path)
		{
			json json;

			std::ifstream file(path);
			file >> json;
			file.close();

			return json;
		}
	};
}
