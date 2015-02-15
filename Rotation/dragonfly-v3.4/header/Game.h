#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Menu.h"
#include <cstring>

#define TYPE_GAME "Game"
#define GAME_TICK_SPEED 5


enum State {
	TUTORIAL,
	GAME,
	PAUSE_MENU,
};

class Game : public ViewObject{
private:
	State state;
	Menu* menu;
	bool isResetting;
public:
//members
	game_state* GameState;
//functions
	Game(Menu* menu, game_state* GameState);
	~Game();
	int eventHandler(Event* e);
	
	State getCurrentState();
	void setCurrentState(State value);
	void draw();
	Menu* getMenu() const;
	void reset();
	void setGameResetFlag(bool value);
};

#endif