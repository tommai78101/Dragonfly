#include "Common.h"
#include <cstring>

#define TYPE_GAME "Game"
#define GAME_TICK_SPEED 5

struct tutorial {
	std::string string;
	int stringLength;
	int stringX;
	int stringY;
	bool floatUp;
	int counter;
	int counterSpeed;
};

struct game_state {
	tutorial TutorialState;
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