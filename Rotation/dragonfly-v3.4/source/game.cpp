#include "..\header\Game.h"
#include "..\header\Menu.h"

Game::Game(){
	new Menu;
	setTransparency();
	setActive(false);
}

int Game::eventHandler(Event* e){
	return 0;
}

void Game::draw(){}
