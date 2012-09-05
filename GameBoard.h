#ifndef GAME_BOARD_H
#define GAME_BOARD_H

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_TIMER;

const int FPS = 60;
const int DEFAULT_WIDTH  = 400; // Default width of display
const int DEFAULT_HEIGHT = 600;  // Default height of display
const int STAIRS_NUM = 10; // The largest number of stairs appear on screen at the same tine
const int STAIRS_COL = 3; // Columns of stairs



class Stair;
class Sprite;

class CGameBoard
{
public:
	enum KEY {KEY_LEFT,KEY_RIGHT};
	CGameBoard(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	~CGameBoard();
	bool init(); // Initialize game engine library
	void reset();// Configure game
	void start();
	void stop();
private:
	// Private method
	void initStair(Stair& stair);
	void initStairsStatus();
	void initSpriteStatus();
	void genStairs(); // Generate new stairs
	void moveStairs();
	void redraw();    // Redraw gameboard

	void collideStair();
	void collideWall();

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

	ALLEGRO_DISPLAY* _pDisplay;
	ALLEGRO_EVENT_QUEUE* _pEventQueue;
	ALLEGRO_TIMER* _pTimer;
	ALLEGRO_TIMER* _pEvolTimer;// Timer for accerating the speed of stairs
};


#endif