#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Menu.h"
#include <cstring>

#define TYPE_GAME "Game"
#define GAME_TICK_SPEED 5

struct board {
	bool rotateCCW;
	bool rotateCW;
	bool isRotating;
	int arrayOrder;
};

struct player_state {
	int x;
	int y;
	int maxX;
	int maxY;
	int minX;
	int minY;
	int initialX;
	int initialY;
};

struct block_state : public player_state {
	bool isMoving;
};

struct stage {
	int size;
	int* layout;
	int width;
	int height;
	block_state* blocks;
};

struct game_state {
	player_state PlayerState;
	stage Stage1;
	board Board;
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
	~Game();
	int eventHandler(Event* e);
	void initializeGameState();
	State getCurrentState();
	void setCurrentState(State value);
	void draw();
};

#endif