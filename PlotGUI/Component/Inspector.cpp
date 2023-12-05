#include "Component/Inspector.h"

namespace PlotGUI
{
	const std::string Inspector::Mark = std::string("##");

	std::string Inspector::TransName(const std::string& fieldName)
	{
		std::string s;

		for (int i = 0; i < fieldName.size(); ++i)
		{
			char c = fieldName[i];

			if (!i)
			{
				c = toupper(c);
			}

			if (isupper(c))
			{
				s += ' ';
			}

			s += c;
		}

		return s;
	}
}
