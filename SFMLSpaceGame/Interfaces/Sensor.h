#pragma once
#include <functional>

class Sensor 
{
public:
	virtual void AddCallback(std::function<void(bool, Sensor*)> callback) = 0;
};