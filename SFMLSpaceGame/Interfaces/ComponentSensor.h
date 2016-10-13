#pragma once
#include <Interfaces/Sensor.h>
class Component;

class ComponentSensor : public Sensor
{
public:
	virtual void AttachComponent(Component* c) = 0;
};