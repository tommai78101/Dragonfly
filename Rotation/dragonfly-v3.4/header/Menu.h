#ifndef MENU_H
#define MENU_H

#include "..\include\ViewObject.h"
#include "..\include\Object.h"
#include "..\include\Event.h"

#define TYPE_MENU "Menu"
#define TYPE_LOGO "Logo"

struct levels {
	int size;
	int currentLevel;
	int maxStageLevel;
	int* stage1;
	int* stage2;
	int* stage3;
	int* stage4;
	int* stage5;
	int* stage6;
	int* stage7;
	int* stage8;
	int* stage9;
	int* stage10;
	int* temp;
};

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
	int previousStageLevel;
};

struct game_state {
	bounds Bounds;
	board Board;
	player_state PlayerState;
	stage Stage1;
	win_state win;
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
	bool isLevelInitialized;

	//Note(Thompson): Make the game work first, and then start worrying about the code layouts.
	struct levels levels;

public:
	Menu();
	~Menu();
	int eventHandler(Event* e);
	void initializeGameState();
	void draw();
	void reset();
	void initializeLevels();
	void nextStage();
	struct levels getLevelState() const;
};

#endif