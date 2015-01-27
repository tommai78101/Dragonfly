#include "..\header\Game.h"
#include "..\header\Menu.h"
#include "..\header\Player.h"
#include "..\header\Border.h"

//TODO(Thompson): Separate the game board (the box) from this class. 
//The game must only handle resetting the game, or quiting the game.
//The game should not manipulate the entities around, it will be up
//for the smaller, specialized classes to work on.

Game::Game(){
	registerInterest(DF_STEP_EVENT);
	registerInterest(DF_KEYBOARD_EVENT);

	initializeGameState();
}

void Game::initializeGameState(){
	this->GameState = {};
	this->GameState.PlayerState.x = 40;
	this->GameState.PlayerState.y = 15;

	this->setCurrentState(State::TUTORIAL);

	Player* player = 0;
	Border* border = 0;

	//TODO(Thompson): May need to do an inline or something sort.
	
	WorldManager& w = WorldManager::getInstance();
	ObjectList list = w.getAllObjects();
	for (ObjectListIterator i(&list); !i.isDone(); i.next()){
		Object* obj = i.currentObject();
		if (obj->getType().compare(TYPE_PLAYER) == 0){
			player = dynamic_cast<Player*>(obj);
		}
		else if (obj->getType().compare(TYPE_BORDER) == 0){
			border = dynamic_cast<Border*>(obj);
		}
	}

	if (player){
		player->setVisible(true);
		player->initializeState(&GameState);
	}
	else {
		new Player(&this->GameState);
	}

	if (border){
		border->setVisible(true);
	}
	else {
		new Border();
	}
}


/*
	NOTE(Thompson): Whenever things are added to the game, they are created on the stack.
	When quitting or removing them, just make them invisible. Do not remove.
	Only when the player really wants to quit, Menu will handle world deletion.

	From Node -> Leaf:
		Menu -> Game -> All game entities.
*/

int Game::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		if (key == 'r'){
			WorldManager& world = WorldManager::getInstance();
			ObjectList list = world.getAllObjects();
			for (ObjectListIterator i(&list); !i.isDone(); i.next()){
				Object* obj = i.currentObject();
				if ((obj->getType().compare(TYPE_MENU) == 0) || (obj->getType().compare(TYPE_LOGO) == 0)){
					obj->setVisible(true);
				}
				else {
					obj->setVisible(false);
				}
			}
		}
		return 1;
	}
	return 0;
}

State Game::getCurrentState(){
	return this->state;
}

void Game::setCurrentState(State value){
	this->state = value;
}

void Game::draw(){
	Object::draw();
}
