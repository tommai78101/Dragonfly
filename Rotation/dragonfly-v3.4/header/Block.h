#ifndef BLOCK_H.

#include "..\header\Common.h"
#include "..\header\Game.h"

class Block : public Object {
private:
	game_state* GameState;
public:
	Block(game_state* GameState);
	int eventHandler(Event* event);
	void draw();
};

#endif