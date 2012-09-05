#ifndef SPRITE_H
#define SPRITE_H

#include "IMovable.h"
#include <assert.h>
#include <allegro5\allegro.h>

struct ALLEGRO_BITMAP;

class Sprite:public IMovable
{
public:
	Sprite(int x,int y,int w=10,int h=10,int spdX=0,int spdY=1);
	virtual ~Sprite();
	inline int getLives() const
	{
		return _lives;
	}
	inline void setLives(const int lives)
	{
		assert(lives>=0);
		_lives = lives;
	}
	inline bool isAlive() const
	{
		return _lives>0?true:false;
	}

	virtual void draw();
private:
	int _lives; // Number of lives the sprite has
	ALLEGRO_COLOR _color;
	ALLEGRO_BITMAP * _bitmap;

};


#endif