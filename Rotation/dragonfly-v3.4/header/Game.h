#include "Common.h"

#define TYPE_GAME "Game"

class Game : public ViewObject{
public:
	Game();
	int eventHandler(Event* e);
	void draw();
};