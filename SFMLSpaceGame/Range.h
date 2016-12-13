#pragma once

template<typename T>
struct Range {
public:
	T min;
	T max;

	template<class Archive>
	void serialize(Archive& ar) 
	{
		ar(CEREAL_NVP(min), CEREAL_NVP(max));
	}
};