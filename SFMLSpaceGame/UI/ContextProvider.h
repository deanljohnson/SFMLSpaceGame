#pragma once
#include <UI/ContextMenu.h>

class ContextProvider 
{
public:
	void SetContextOptions(ContextMenu& contextMenu, const Item& item);
};