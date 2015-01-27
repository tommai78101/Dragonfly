#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include <cstring>

#define TYPE_GAME "Game"
#define GAME_TICK_SPEED 5

struct player_state {
	int x;
	int y;
};

struct game_state {
	player_state PlayerState;
};

enum State {
	TUTORIAL,
	GAME,
	PAUSE_MENU,
};

class Game : public ViewObject{
private:
	State state;
public:
//members
	game_state GameState;
//functions
	Game();
	int eventHandler(Event* e);
	void initializeGameState();
	State getCurrentState();
	void setCurrentState(State value);
	void draw();
};

#endif