#ifndef EXIT_H
#define EXIT_H

#define TYPE_EXIT "Exit"

#include "Common.h"
#include "Game.h"

class Exit : public Object {
private:
	game_state* GameState;
public:
	Exit(game_state* GameState);
	~Exit();
	int eventHandler(Event* e);
	void draw();
};

#endif