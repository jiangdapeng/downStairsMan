#include "Sprite.h"
#include "basics.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <iostream>

Sprite::Sprite(int x,int y,int w,int h,int spdX,int spdY)
		:IMovable(x,y,w,h,spdX,spdY)
{

	_lives = 3;
	_bitmap = NULL;
	_color = al_map_rgb(0,255,0);
	//al_clear_to_color(color);
}

Sprite::~Sprite()
{
	if(_bitmap != NULL)
		al_destroy_bitmap(_bitmap);
	_bitmap = NULL;
}

void Sprite::draw()
{
	al_draw_filled_rectangle(getX(),getY(),getX()+getW(),getY()+getH(),_color);
}