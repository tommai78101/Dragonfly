#ifndef PLAYER_H

#include "Common.h"
#include "Game.h"

#define TYPE_PLAYER "Player"

class Player : public Object {
private:
	game_state* GameState;
public:
	Player(game_state* GameState);
	int eventHandler(Event* e);
	void draw();
	void initializeState(game_state* GameState);
	void setGameBounds(int x, int y, int w, int h);
};

#define PLAYER_H
#endif