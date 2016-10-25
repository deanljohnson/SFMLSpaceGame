#pragma once
#include <string>

namespace StringDisplayHelper
{
	inline std::string GetRenderedString(unsigned int number)
	{
		std::stringstream ss;
		ss << number;

		return ss.str();
	}
}