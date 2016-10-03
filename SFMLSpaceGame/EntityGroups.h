#pragma once
#include <Entity.h>

//NOTE: Entity's render method is called according to group number order

const Group BACKGROUND_GROUP{ 0 };
const Group PROJECTILE_GROUP{ 1 };
const Group PLAYER_GROUP{ 2 };
const Group NON_PLAYER_SHIP_GROUP{ 3 };
const Group UI_GROUP{ 4 };