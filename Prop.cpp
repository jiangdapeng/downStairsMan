#include "Prop.h"

void CProp::draw()
{
	// TODO:override this method
}

bool CProp::isCollide(const CProp& other)
{
	// Collision check
	// Rectangle collision
	if(getX()>other.getX()+other.getW() || 
	   getX()+getW()<other.getX() ||
	   getY()>other.getY()+other.getH() ||
	   getY()+getH()<other.getY())
	{
		return false;
	}
	return true;
}