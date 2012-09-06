#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include "Control.h"

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_TIMER;

const int FPS = 60;
const int LEVEL_TIME = 20;
const int WINDOW_WIDTH  = 600; // Default width of display
const int WINDOW_HEIGHT = 600; // Default height of display
const int DISPLAY_WIDTH = 200; // Display area
const int DEFAULT_CONTROL_HEIGHT = 50;

const int STAIRS_NUM = 10; // The largest number of stairs appear on screen at the same tine
const int STAIRS_COL = 3; // Columns of stairs

class Stair;
class Sprite;

class CGameBoard
{
public:
	enum KEY {KEY_LEFT,KEY_RIGHT};
	CGameBoard(int width = WINDOW_WIDTH, int height = WINDOW_HEIGHT);
	~CGameBoard();
	bool init(); // Initialize game engine library
	void reset();// Configure game
	void start();
	void stop();
	void initGameBoard();
private:
	// Private method
	void initStair(Stair& stair,bool resetSpeed=true);
	void initStairsStatus();
	void initSpriteStatus();
	void genStairs(); // Generate new stairs
	void moveStairs();
	void redraw();    // Redraw gameboard

	void collideStair();
	void collideWall();

	bool showGameOver();

private:
	// Member variables
	int _width; // Width of gameboard
	int _height;// Height of gameboard

	bool _isGameOver;
	bool _redraw;

	bool _keyPressed[2];

	Sprite* _pMan;
	Stair* _pStairs;//
	bool _isStairsLive[STAIRS_NUM]; // flags show whether a stair has been out of boundary

	CControl* _levelArea; // Control show the current level of the game
	CControl* _timeArea;  // show the time passed since begin

	CControl* _scoreArea;
	CControl* _playerInfo;

	std::vector<CControl*> _allControls;// for convienience of managing controls 

	ALLEGRO_DISPLAY* _pDisplay;
	ALLEGRO_EVENT_QUEUE* _pEventQueue;
	ALLEGRO_TIMER* _pTimer;
	ALLEGRO_TIMER* _pEvolTimer;// Timer for accerating the speed of stairs
	ALLEGRO_FONT* _pFont;

	int _timeCount;
	int _level;
	//int _score;
};

#endif