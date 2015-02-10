#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Menu.h"
#include <cstring>

#define TYPE_GAME "Game"
#define GAME_TICK_SPEED 5

struct win_state {
	bool win;
	bool isGameWinCreated;
};

struct exit_state{
	int x;
	int y;
	bool isBlocked;
};

struct bounds {
	int minX;
	int minY;
	int maxX;
	int maxY;
};

struct board {
	bool rotateCCW;
	bool rotateCW;
	bool isRotating;
	int arrayOrder;
};

struct player_state {
	int x;
	int y;
	int initialX;
	int initialY;
};

struct block_state : public player_state {
	bool isMoving;
	int id;
};

struct stage {
	int size;
	int width;
	int height;
	int* layout;
	block_state* blocks;
	int blockStateSize;
	exit_state exit;
	win_state win;
};

struct game_state {
	bounds Bounds;
	board Board;
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
	~Game();
	int eventHandler(Event* e);
	void initializeGameState();
	State getCurrentState();
	void setCurrentState(State value);
	void draw();
	Menu* getMenu() const;
	void reset();
};

#endif