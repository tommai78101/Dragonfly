#include "Common.h"

class Game : public ViewObject{
public:
	Game();
	int eventHandler(Event* e);
	void draw();
};