#include "GameBoard.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "Stair.h"
#include "Sprite.h"
#include "basics.h"
#include <iostream>
using namespace std;


CGameBoard::CGameBoard(int width, int height):
	_pDisplay(NULL),_pEventQueue(NULL),_pTimer(NULL)
{
	_width = width;
	_height = height;
	_isGameOver = false;
	_redraw = true;
	_pMan = NULL;
	_pStairs = NULL;
	_levelArea = NULL;
	_timeArea = NULL;
	_scoreArea = NULL;
	_playerInfo = NULL;
	_keyPressed[KEY_LEFT] = false;
	_keyPressed[KEY_RIGHT] = false;
	_pFont = NULL;
	_pEvolTimer = NULL;
}

CGameBoard::~CGameBoard()
{
	if(_pMan)
		delete _pMan;
	if(_pStairs)
		delete []_pStairs;
	for(auto it=_allControls.begin();it!=_allControls.end();++it)
	{
		delete *it;
	}
	if(_pEventQueue)
		al_destroy_event_queue(_pEventQueue);
	if(_pTimer)
		al_destroy_timer(_pTimer);
	if(_pEvolTimer)
		al_destroy_timer(_pEvolTimer);
	if(_pDisplay)
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
		return false;
	}
	if ( !al_install_keyboard() )
	{
		return false;
	}
	
	al_init_font_addon();
	al_init_ttf_addon();
	
	_pFont = al_load_font("pirulen.ttf",18,0);
	if(_pFont == NULL)
	{
		cout<<"font failed"<<endl;
		return false;
	}
	
	// Initialize event queue and timer
	_pEventQueue = al_create_event_queue();
	if ( _pEventQueue == NULL )
	{
		return false;
	}
	_pTimer = al_create_timer(1.0/FPS);
	_pEvolTimer = al_create_timer(LEVEL_TIME);
	if ( _pTimer == NULL || _pEvolTimer == NULL)
	{
		return false;
	}
	// Register event source
	al_register_event_source( _pEventQueue, al_get_keyboard_event_source() );
	al_register_event_source( _pEventQueue, al_get_display_event_source(_pDisplay) );
	al_register_event_source( _pEventQueue, al_get_timer_event_source(_pTimer) );
	al_register_event_source( _pEventQueue, al_get_timer_event_source(_pEvolTimer));

	return true;
}

void CGameBoard::initGameBoard()
{
	_timeCount=0;
	_level = 1;

	int startx = WINDOW_WIDTH - DISPLAY_WIDTH;
	_levelArea = new CControl();
	_levelArea->setX(startx);
	_levelArea->setY(10);
	_levelArea->setText("LEVEL:1");
	_levelArea->setFont(_pFont);
	
	_timeArea = new CControl();
	_timeArea->setX(startx);
	_timeArea->setY(100);
	_timeArea->setText("TIME:0");
	_timeArea->setFont(_pFont);

	_scoreArea = new CControl();
	_scoreArea->setX(startx);
	_scoreArea->setY(200);
	_scoreArea->setText("SCORE:0");
	_scoreArea->setFont(_pFont);

	_playerInfo = new CControl();
	_playerInfo->setX(startx);
	_playerInfo->setY(300);
	_playerInfo->setText("LIVES:3");
	_playerInfo->setFont(_pFont);

	_allControls.push_back(_levelArea);
	_allControls.push_back(_timeArea);
	_allControls.push_back(_scoreArea);
	_allControls.push_back(_playerInfo);

	initSpriteStatus();
	initStairsStatus();
	//cout<<"init gameboard ok"<<endl;
}

void CGameBoard::reset()
{
}

void CGameBoard::start()
{
	srand(time(NULL));
	char tmp[20];
	bool done = false;
	al_start_timer(_pTimer);
	al_start_timer(_pEvolTimer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(_pEventQueue,&ev);
		if( ev.type == ALLEGRO_EVENT_TIMER )
		{
			if(ev.timer.source == _pTimer)
			{
				_redraw = true;
				// Redraw timer
				if(!_isGameOver)
				{
					_timeCount++;
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

					if(_pMan->getY()>WINDOW_HEIGHT || _pMan->getY()<0)
					{
						_pMan->setLives(_pMan->getLives()-1);
						_pMan->setX(WINDOW_WIDTH/2);
						_pMan->setY(WINDOW_HEIGHT/2);

						sprintf(tmp,"LIVES:%d",_pMan->getLives());
						_playerInfo->setText(tmp);
					}

					sprintf(tmp,"TIME:%.2fs",_timeCount/60.0);
					_timeArea->setText(tmp);

					sprintf(tmp,"SCORE:%d",_timeCount);
					_scoreArea->setText(tmp);

					if(_pMan->isAlive()==false)
					{
						_isGameOver = true;
					}
				}
			}
			else
			{
				// EvolTimer
				if(!_isGameOver)
				{
					for(int i=0;i<STAIRS_NUM;++i)
					{
						_pStairs[i].setSpeedY(_pStairs[i].getSpeedY()-2);
					}
					sprintf(tmp,"LEVEL:%d",++_level);
					_levelArea->setText(tmp);
				}
			}
		}
		else if( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
		{
			// Close button clicked
			done = true;
		}
		else if( ev.type == ALLEGRO_EVENT_KEY_DOWN )
		{
			// Key pressed
			switch( ev.keyboard.keycode )
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				_keyPressed[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				_keyPressed[KEY_RIGHT] = true;
				break;
			case ALLEGRO_KEY_Y:// again? yes
				done = false;
				_isGameOver = false;
				initGameBoard();
				break;
			case ALLEGRO_KEY_N:// again? no
				done = true;
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
			if(!_isGameOver)
			{
				redraw();
			}
			else
			{
				al_draw_text(_pFont,al_map_rgb(255,0,0),150,200,0,"GAME OVER!");
				al_draw_textf(_pFont,al_map_rgb(0,0,255),150,300,0,"FINAL SCORE:%d",_timeCount);
				al_draw_text(_pFont,al_map_rgb(0,0,255),150,400,0,"TRY AGAIN?(Y/N)");
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(255,255,255));
		}
	}
}

void CGameBoard::stop()
{
}

void CGameBoard::initStair(Stair& stair,bool resetSpeed)
{
	// Generate a stair partialy randomly
	int idx = rand()%3;
	int step = (WINDOW_WIDTH-DISPLAY_WIDTH)/STAIRS_COL;
	int start = (step-stair.getW())/2;
	stair.setX(idx*step + start);
	stair.setY(WINDOW_HEIGHT); // Stairs start from the bottom
	if(resetSpeed)
		stair.setSpeedY(-2);
}

void CGameBoard::initStairsStatus()
{
	// Only generate one stair at the begin of game
	if(_pStairs == NULL)
			_pStairs = new Stair[STAIRS_NUM];
	for(int i=0;i<STAIRS_NUM;++i)
	{
		initStair(_pStairs[i]);
	}
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
			initStair(_pStairs[i],false);
			_isStairsLive[i] = true;
			break;
		}
	}
}

void CGameBoard::initSpriteStatus()
{
	// TODO(asuwill.jdp@gmail.com):
	if(_pMan == NULL)
		_pMan = new Sprite(WINDOW_WIDTH/2,0);
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
	// player
	_pMan->draw();
	// stairs
	for(int i=0;i<STAIRS_NUM;++i)
	{
		if( _isStairsLive[i] )
			_pStairs[i].draw();
	}
	// display controls
	for(auto it = _allControls.begin();it!=_allControls.end();++it)
	{
		(*it)->draw();
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
			return;
		}
	}
	// No collision
	_pMan->setSpeedY(1);
}

void CGameBoard::collideWall()
{
	if(_pMan->getX()<0)
		_pMan->setX(0);
	else if(_pMan->getX()+_pMan->getW()>WINDOW_WIDTH-DISPLAY_WIDTH)
		_pMan->setX(WINDOW_WIDTH-DISPLAY_WIDTH-_pMan->getW());
}

bool CGameBoard::showGameOver()
{
	/*
	CControl tmp;
	tmp.setBGColor(al_map_rgb(0,0,255));
	tmp.setFGColor(al_map_rgb(255,255,255));
	tmp.setText("GAME OVER!");
	ALLEGRO_FONT* font=al_load_font("pirulen.ttf",30,0);
	tmp.setFont(font);
	tmp.setX(200);
	tmp.setY(200);
	tmp.setW(100);
	tmp.setH(100);
	*/
	return true;
}