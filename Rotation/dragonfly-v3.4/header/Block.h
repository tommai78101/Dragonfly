#ifndef BLOCK_H

#include "../header/Common.h"
#include "../header/Game.h"

#define TYPE_BLOCK "Block"


class Block : public Object {
private:
	game_state* GameState;
	int id;
public:
	Block(game_state* GameState, int id);
	int eventHandler(Event* event);
	void draw();

	void setBlockID(int value);
	int getBlockID() const;
};

#endif