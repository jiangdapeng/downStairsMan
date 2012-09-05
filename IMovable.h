#ifndef IMOVALE_H
#define IMOVALE_H

#include "Prop.h"

class IMovable:public CProp
{
public:
	IMovable(int x,int y,int w,int h,int spdX,int spdY):CProp(x,y,w,h),_speedX(spdX),_speedY(spdY){}
	virtual ~IMovable();
	virtual void move();
	virtual void moveBack();
	inline int getSpeedX() const
	{
		return _speedX;
	}
	inline void setSpeedX(const int spdX)
	{
		_speedX = spdX;
	}
	inline int getSpeedY() const
	{
		return _speedY;
	}
	inline void setSpeedY(const int spdY)
	{
		_speedY = spdY;
	}
private:
	int _speedX;
	int _speedY;
};

#endif