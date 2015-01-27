#ifndef PLAYER_H

#include "Common.h"
#include "Game.h"

#define TYPE_PLAYER "Player"

class Player : public Object {
private:
	player_state PlayerState;
public:
	Player(game_state* GameState);
	int eventHandler(Event* e);
	void draw();
	void setPlayerState(player_state State);
	player_state getPlayerState() const;
};

#define PLAYER_H
#endif