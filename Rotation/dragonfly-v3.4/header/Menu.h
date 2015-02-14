#ifndef MENU_H
#define MENU_H

#include "..\include\ViewObject.h"
#include "..\include\Object.h"
#include "..\include\Event.h"

#define TYPE_MENU "Menu"
#define TYPE_LOGO "Logo"


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
	int currentStageLevel;
};

struct game_state {
	bounds Bounds;
	board Board;
	player_state PlayerState;
	stage Stage1;
	int stageLevel;
	win_state win;
};

struct levels {
	int size;
	int* stage1;
	int* stage2;
};


class Logo : public Object {
private:
	int goUp;
public:
	Logo(Position pos);
	int eventHandler(Event* e);
	void draw();
};

class Menu : public Object {
private:
	bool initalSpin;
	Position cursorPosition;
	bool canSelectOptions;
	bool StartGame;
	game_state GameState;
public:
	Menu();
	~Menu();
	int eventHandler(Event* e);
	void initializeGameState();
	void draw();
	void reset();
};

#endif