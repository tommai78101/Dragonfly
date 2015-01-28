#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Menu.h"
#include <cstring>

#define TYPE_GAME "Game"
#define GAME_TICK_SPEED 5

struct stage {
	int size;
	int* layout; //Array
	int width;
	int height;
};

struct player_state {
	int x;
	int y;
	int maxX;
	int maxY;
	int minX;
	int minY;
};

struct game_state {
	player_state PlayerState;
	stage Stage1;
};

enum State {
	TUTORIAL,
	GAME,
	PAUSE_MENU,
};

class Game : public ViewObject{
private:
	State state;
	Menu* menu;
public:
//members
	game_state GameState;
//functions
	Game(Menu* menu);
	int eventHandler(Event* e);
	void initializeGameState();
	State getCurrentState();
	void setCurrentState(State value);
	void draw();
};

#endif