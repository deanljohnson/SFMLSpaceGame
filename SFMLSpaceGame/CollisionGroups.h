#pragma once

const int BULLET_COLLISION_GROUP = -1;

// these define bitmasks to be used in Box2d
// for collision filtering. The IS_XXX apply
// to a fixture's category masks and they define
// what an object is. The COLLIDES_WITH_XXX
// define what categories an object should
// collide with and should be assigned to a
// fixtures mask bits.

const int IS_SHIP =					0x0001;
const int IS_BULLET =				0x0002;
const int IS_STATION =				0x0004;
const int IS_SENSOR =				0x0008;
const int COLLIDES_WITH_SHIP =		0x0001;
const int COLLIDES_WITH_BULLET =	0x0002;
const int COLLIDES_WITH_STATION =	0x0004;
const int COLLIDES_WITH_SENSOR =	0x0008;