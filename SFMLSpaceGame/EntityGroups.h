#pragma once

//NOTE: Entity's update/render methods are called according to group number order

const Group BACKGROUND_GROUP{ 0 };
const Group PROJECTILE_GROUP{ 1 };
const Group EXPLOSION_GROUP{ 2 };
const Group ASTEROID_GROUP{ 3 };
const Group PICKUP_GROUP{ 4 };
const Group PLAYER_GROUP{ 5 };
const Group NON_PLAYER_SHIP_GROUP{ 6 };
const Group STATION_GROUP{ 7 };
const Group UI_GROUP{ 8 };