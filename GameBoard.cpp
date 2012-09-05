#include "GameBoard.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include "Stair.h"
#include "Sprite.h"
#include "basics.h"
#include <iostream>
using namespace std;

// Color


CGameBoard::CGameBoard(int width, int height):
	_pDisplay(NULL),_pEventQueue(NULL),_pTimer(NULL)
{
	_width = width;
	_height = height;
	_isGameOver = false;
	_redraw = true;
	_pMan = NULL;
	_pStairs = NULL;
	_keyPressed[KEY_LEFT] = false;
	_keyPressed[KEY_RIGHT] = false;
}

CGameBoard::~CGameBoard()
{
	if(_pMan)
		delete _pMan;
	if(_pStairs)
		delete []_pStairs;
	al_destroy_event_queue(_pEventQueue);
	al_destroy_timer(_pTimer);
	al_destroy_display(_pDisplay);
}

bool CGameBoard::init()
{
	// Initialize Allegro functions
	if ( !al_init() )
		return false;

	// Create display
	_pDisplay = al_create_display(_width,_height);
	if ( _pDisplay == NULL)
		return false;

	// Initialize Allegro Module
	if ( !al_init_primitives_addon() )
	{
		al_destroy_display(_pDisplay);
		return false;
	}
	if ( !al_install_keyboard() )
	{
		al_destroy_display(_pDisplay);
		return false;
	}

	// Initialize event queue and timer
	_pEventQueue = al_create_event_queue();
	if ( _pEventQueue == NULL )
	{
		al_destroy_display(_pDisplay);
		return false;
	}
	_pTimer = al_create_timer(1.0/FPS);
	_pEvolTimer = al_create_timer(10);
	if ( _pTimer == NULL || _pEvolTimer == NULL)
	{
		al_destroy_event_queue(_pEventQueue);
		al_destroy_display(_pDisplay);
		return false;
	}
	// Register event source
	al_register_event_source( _pEventQueue, al_get_keyboard_event_source() );
	al_register_event_source( _pEventQueue, al_get_display_event_source(_pDisplay) );
	al_register_event_source( _pEventQueue, al_get_timer_event_source(_pTimer) );
	al_register_event_source( _pEventQueue, al_get_timer_event_source(_pEvolTimer));

	_pMan = new Sprite(DEFAULT_WIDTH/2,0);
	_pStairs = new Stair[STAIRS_NUM];

	return true;
}

void CGameBoard::reset()
{
	initSpriteStatus();
	initStairsStatus();
}

void CGameBoard::start()
{
	srand(time(NULL));
	al_start_timer(_pTimer);
	al_start_timer(_pEvolTimer);
	while(!_isGameOver)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(_pEventQueue,&ev);
		if( ev.type == ALLEGRO_EVENT_TIMER )
		{
			if(ev.timer.source == _pTimer)
			{
				// Redraw timer
				_redraw = true;
				if(_keyPressed[KEY_LEFT])
				{
					_pMan->setSpeedX(-2);
				}
				if(_keyPressed[KEY_RIGHT])
				{
					_pMan->setSpeedX(2);
				}
				_pMan->move();
				moveStairs();
				collideWall();
				collideStair();
				if(rand()%30 == 0)
					genStairs();

				if(_pMan->getY()>DEFAULT_HEIGHT || _pMan->getY()<0)
				{
					_pMan->setLives(_pMan->getLives()-1);
					_pMan->setX(DEFAULT_WIDTH/2);
					_pMan->setY(DEFAULT_HEIGHT/2);
				}

				if(_pMan->isAlive()==false)
					_isGameOver = true;
			}
			else
			{
				// EvolTimer
				for(int i=0;i<STAIRS_NUM;++i)
				{
					_pStairs[i].setSpeedY(_pStairs[i].getSpeedY()-2);
				}
			}
		}
		else if( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
		{
			// Close button clicked
			_isGameOver = true;
		}
		else if( ev.type == ALLEGRO_EVENT_KEY_DOWN )
		{
			// Key pressed
			switch( ev.keyboard.keycode )
			{
			case ALLEGRO_KEY_ESCAPE:
				_isGameOver = true;
				break;
			case ALLEGRO_KEY_LEFT:
				_keyPressed[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				_keyPressed[KEY_RIGHT] = true;
				break;
			default:
				break;
			}
		}
		else if( ev.type == ALLEGRO_EVENT_KEY_UP )
		{
			// Key released
			switch( ev.keyboard.keycode )
			{
			case ALLEGRO_KEY_LEFT:
				_keyPressed[KEY_LEFT]= false;
				break;
			case ALLEGRO_KEY_RIGHT:
				_keyPressed[KEY_RIGHT] = false;
				break;
			default:
				break;
			}
		}

		if( _redraw && al_is_event_queue_empty(_pEventQueue) )
		{
			_redraw = false; // Wait for next timer
			redraw();
			al_flip_display();
			al_clear_to_color(al_map_rgb(255,255,255));
		}
	}
}

void CGameBoard::stop()
{
}

void CGameBoard::initStair(Stair& stair)
{
	// Generate a stair partialy randomly
	int idx = rand()%3;
	int step = DEFAULT_WIDTH/STAIRS_COL;
	int start = (step-stair.getW())/2;
	stair.setX(idx*step + start);
	stair.setY(DEFAULT_HEIGHT); // Stairs start from the bottom
}

void CGameBoard::initStairsStatus()
{
	// Only generate one stair at the begin of game
	initStair(_pStairs[0]);
	_isStairsLive[0] = true;
	for(int i=1;i<STAIRS_NUM;++i)
	{
		_isStairsLive[i] = false;
	}
}

void CGameBoard::genStairs()
{
	// Generate one stair a time
	for(int i=0;i<STAIRS_NUM;++i)
	{
		if( !_isStairsLive[i] )
		{
			initStair(_pStairs[i]);
			_isStairsLive[i] = true;
			break;
		}
	}
}

void CGameBoard::initSpriteStatus()
{
	// TODO(asuwill.jdp@gmail.com):
	_pMan->setSpeedX(0);
	_pMan->setSpeedY(2);
	_pMan->setLives(3);
}

void CGameBoard::moveStairs()
{
	for(int i=0;i<STAIRS_NUM;++i)
	{
		if(_isStairsLive[i])
			_pStairs[i].move();
		if(_pStairs[i].getY()<0)
			_isStairsLive[i] = false;
	}
}

void CGameBoard::redraw()
{
	_pMan->draw();
	for(int i=0;i<STAIRS_NUM;++i)
	{
		if( _isStairsLive[i] )
			_pStairs[i].draw();
	}
}

void CGameBoard::collideStair()
{
	for(int i=0;i<STAIRS_NUM;++i)
	{
		if(_pMan->isCollide(_pStairs[i]))
		{
			// Set _pMan stand on the stair
			_pMan->setY(_pStairs[i].getY()-_pMan->getH());
			_pMan->setSpeedY(_pStairs[i].getSpeedY());
			break;
		}
	}
	// No collision
	_pMan->setSpeedY(1);
}

void CGameBoard::collideWall()
{
	if(_pMan->getX()<0)
		_pMan->setX(0);
	else if(_pMan->getX()+_pMan->getW()>DEFAULT_WIDTH)
		_pMan->setX(DEFAULT_WIDTH-_pMan->getW());
}