#include "..\header\Player.h"

Player::Player(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	ResourceManager& r = ResourceManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	if (!l.isStarted() || !r.isStarted() || !w.isStarted()){
		l.writeLog("Manager was not started. Order by: %s %s %s", BoolToString(l.isStarted()), BoolToString(r.isStarted()), BoolToString(w.isStarted()));
		w.markForDelete(this);
		return;
	}
	
	//No need for a sprite. A simple character will do.
	setType(TYPE_PLAYER);
	setTransparency();
	setSolidness(Solidness::HARD);
	Position pos(GameState->PlayerState.x, GameState->PlayerState.y);
	setPosition(pos);

	registerInterest(DF_KEYBOARD_EVENT);
	registerInterest(DF_STEP_EVENT);

	this->GameState = GameState;
	setVisible(true);
}


//TODO: Add collision detection/response.
int Player::eventHandler(Event* e){
	LogManager& l = LogManager::getInstance();
	if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		switch (key){
			case 'a':{
				int x = this->getPosition().getX();
				this->setPosition(Position(--x, this->getPosition().getY()));
				break;
			}
			case 'd':{
				int x = this->getPosition().getX();
				this->setPosition(Position(++x, this->getPosition().getY()));
				break;
			}
			default:{
				return 0;
			}
		}
		return 1;
	}
	else if (e->getType() == DF_STEP_EVENT){
		this->GameState->PlayerState.x = this->getPosition().getX();
		this->GameState->PlayerState.y = this->getPosition().getY();
		return 1;
	}
	return 0;
}

void Player::draw(){
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	g.drawCh(this->getPosition(), '@', 0);
	return;
}

void Player::initializeState(game_state* GameState){
	this->GameState = GameState;
	return;
}
