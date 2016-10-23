#pragma once
#include <Components/Component.h>

class Inventory : public Component
{
private:
	int m_credits;
public:
	
	int GetCredits();
	void SetCredits(int c);
};