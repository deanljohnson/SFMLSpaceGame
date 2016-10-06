#pragma once
#include <string>

struct UI_Result
{
	UI_Result(const UI_Result& other)
		: booleanValue{ other.booleanValue },
		intValue{ other.intValue },
		stringValue{ other.stringValue }
	{
	}

	UI_Result(UI_Result&& other)
		: booleanValue{ other.booleanValue },
		intValue{ other.intValue },
		stringValue{ move(other.stringValue) }
	{
	}

	UI_Result& operator=(UI_Result other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

	UI_Result() { }
	~UI_Result() {}

	bool booleanValue;
	int intValue;
	std::string stringValue;
};