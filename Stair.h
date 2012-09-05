#ifndef STAIR_H
#define STAIR_H

#include "IMovable.h"
#include <allegro5\allegro.h>

class Stair:public IMovable
{
public:
	Stair(int x=0,int y=0,int w=50,int h=5,int spdX=0,int spdY=-2);
	~Stair();
	inline ALLEGRO_COLOR getColor() const
	{
		return _color;
	}
	void setColor(const ALLEGRO_COLOR& color);
	virtual void draw();
private:
	ALLEGRO_COLOR _color;
};

#endif