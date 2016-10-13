#include <Components/Component.h>
#include <Interfaces/Sensor.h>

void Component::AttachToSensor(Sensor* sensor) 
{
	sensor->AddCallback(
		[this](bool state, Sensor* sensor)
		{
			SetActive(state);
		});
}