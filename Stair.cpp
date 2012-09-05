#include "Stair.h"
#include "basics.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

Stair::Stair(int x,int y,int w,int h,int spdX,int spdY)
		:IMovable(x,y,w,h,spdX,spdY)
{
	setColor(al_map_rgb(155,155,0));
}

Stair::~Stair()
{
}

void Stair::setColor(const ALLEGRO_COLOR& color)
{
	_color = color;
}

void Stair::draw()
{
	al_draw_filled_rectangle(getX(),getY(),getX()+getW(),getY()+getH(),_color);
}