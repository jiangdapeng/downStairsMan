#include "IMovable.h"

IMovable::~IMovable()
{
}

void IMovable::move()
{
	setOldX(getX()); // Keep current position before movement
	setOldY(getY()); // Keep current position before movement
	setX(getX()+getSpeedX());
	setY(getY()+getSpeedY());
}

void IMovable::moveBack()
{
	setX(getOldX()); // stand one step back
	setY(getOldY());
}