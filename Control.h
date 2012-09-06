#pragma once
#include "prop.h"
#include <string>
#include <allegro5\allegro.h>

struct ALLEGRO_FONT;

class CControl :
	public CProp
{
public:
	// To show or not to show
	enum DISPLAY_STATE{DS_SHOW,DS_HIDE};
	CControl(void);
	virtual ~CControl(void);

	////////////////////////////////////////////////////
	// param[in] text:
	//      the text to display on control
	// return:
	//      void
	////////////////////////////////////////////////////
	void setText(const std::string& text);

	void setText(const char* text);

	////////////////////////////////////////////////////
	// param[in] color:
	//      the new background color
	// return:
	//      the old backgound color
	////////////////////////////////////////////////////
	ALLEGRO_COLOR setBGColor(const ALLEGRO_COLOR& color);

	////////////////////////////////////////////////////
	// param[in] color:
	//      the new forground color
	// return:
	//      the old forground color
	////////////////////////////////////////////////////
	ALLEGRO_COLOR setFGColor(const ALLEGRO_COLOR& color);

	////////////////////////////////////////////////////
	// param[in] font:
	//      new font used to display text
	// return:
	//      the old font
	////////////////////////////////////////////////////
	ALLEGRO_FONT* setFont(ALLEGRO_FONT* const font);

	////////////////////////////////////////////////////
	// param[in] state:
	//      the display state DS_SHOW or DS_HIDE
	// return:
	//      void
	////////////////////////////////////////////////////
	void setDispState(DISPLAY_STATE state);

	std::string getText() const;
	ALLEGRO_COLOR getBGColor()const;
	ALLEGRO_COLOR getFGColor()const;

	////////////////////////////////////////////////////
	// param:
	//      None
	// return:
	//      void
	// Note: override function
	////////////////////////////////////////////////////
	virtual void draw();
	
private:
	std::string _text;// 
	ALLEGRO_COLOR _bgColor;
	ALLEGRO_COLOR _fgColor;
	ALLEGRO_FONT* _font;
	DISPLAY_STATE _dispState; // Whether display on the screen or not
};

