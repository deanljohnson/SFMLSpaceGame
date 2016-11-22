#pragma once
#include <string>

namespace StringHelper
{
	inline std::string GetRenderedString(unsigned int number)
	{
		std::stringstream ss;
		ss << number;

		return ss.str();
	}

	inline bool StrEndsWith(const std::string& value, const std::string& ending)
	{
		if (ending.size() > value.size()) return false;
		return equal(ending.rbegin(), ending.rend(), value.rbegin());
	}
}