#include "Control.h"
#include "basics.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
using namespace std;

CControl::CControl(void)
{
	_text = "";
	_bgColor = al_map_rgb(0,0,0);
	_fgColor = al_map_rgb(255,255,255);
	_font = NULL;
	_dispState = DS_SHOW;

	_width = 200;
	_height = 50;
}


CControl::~CControl(void)
{
}

void CControl::setText(const string& text)
{
	_text = text;
}

void CControl::setText(const char* text)
{
	_text = string(text);
}

ALLEGRO_COLOR CControl::setBGColor(const ALLEGRO_COLOR& color)
{
	ALLEGRO_COLOR old = _bgColor;
	_bgColor = color;
	return old;
}

ALLEGRO_COLOR CControl::setFGColor(const ALLEGRO_COLOR& color)
{
	ALLEGRO_COLOR old = _fgColor;
	_fgColor = color;
	return old;
}

ALLEGRO_FONT* CControl::setFont(ALLEGRO_FONT* const font)
{
	ALLEGRO_FONT* old = _font;
	_font = font;
	return old;
}

void CControl::setDispState(DISPLAY_STATE state)
{
	_dispState = state;
}

string CControl::getText() const
{
	return _text;
}

ALLEGRO_COLOR CControl::getBGColor() const
{
	return _bgColor;
}

ALLEGRO_COLOR CControl::getFGColor() const
{
	return _fgColor;
}

void CControl::draw()
{
	switch(_dispState)
	{
	case DS_SHOW:
		// Draw background
		al_draw_filled_rectangle(getX(),getY(),getX()+getW(),getY()+getH(),_bgColor);
		// Draw text
		al_draw_text(_font,_fgColor,getX(),getY(),0,_text.c_str());
		break;
	case DS_HIDE:
		break;
	}
}