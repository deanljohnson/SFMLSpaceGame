#pragma once
#include <functional>

class Sensor 
{
public:
	virtual ~Sensor()
	{
	}

	virtual void AddCallback(std::function<void(bool, Sensor*)> callback) = 0;
};