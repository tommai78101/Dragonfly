#ifndef BORDER_H

#define TYPE_BORDER "Border"

#include "Common.h"

class Border : public Object {
private:
	int width;
	int height;
public:
	Border(game_state* GameState);
	int eventHandler(Event* e);
	void draw();
	int getWidth();
	int getHeight();
};

#define BORDER_H
#endif