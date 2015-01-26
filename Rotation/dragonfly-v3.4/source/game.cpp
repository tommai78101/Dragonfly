#include "..\header\Game.h"
#include "..\header\Menu.h"


inline bool GameTick(game_state* GameState, int Threshold){
	bool Result = false;
	tutorial* Tutorial = &GameState->TutorialState;
	if (Tutorial->counterSpeed > GAME_TICK_SPEED){
		Tutorial->counter++;
		Tutorial->counterSpeed = 0;
	}
	else {
		Tutorial->counterSpeed++;
	}

	if (Tutorial->counter > Threshold){
		Result = true;
	}
	return Result;
}

Game::Game(){
	this->GameState = {};
	this->setCurrentState(State::TUTORIAL);

	GraphicsManager& g = GraphicsManager::getInstance();
	tutorial* Tutorial = &this->GameState.TutorialState;
	Tutorial->stringX = g.getHorizontal() / 4 + 2;
	Tutorial->stringY = g.getVertical() / 2;
	Tutorial->floatUp = false;
	Tutorial->counterSpeed = 5;

	setSpriteSlowdown(5);

	registerInterest(DF_STEP_EVENT);
	registerInterest(DF_KEYBOARD_EVENT);
}

int Game::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		if (this->getCurrentState() == State::TUTORIAL){
			tutorial* Tutorial = &this->GameState.TutorialState;
			if (Tutorial->counterSpeed == 0){
				if (Tutorial->floatUp){
					Tutorial->stringY--;
				}
				else {
					Tutorial->stringY++;
				}
			}

			if (GameTick(&this->GameState, 3)){
				Tutorial->floatUp = !Tutorial->floatUp;
				Tutorial->counter = 0;
			}
		}
	}
	else if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		if (key == 'r'){
			Object* menu;
			Object* logo;
			WorldManager& world = WorldManager::getInstance();
			ObjectList list = world.getAllObjects();
			for (ObjectListIterator i(&list); !i.isDone(); i.next()){
				Object* obj = i.currentObject();
				if (obj->getType().compare(TYPE_MENU) == 0){
					menu = obj;
				}
				else if (obj->getType().compare(TYPE_LOGO) == 0){
					logo = obj;
				}
			}
			menu->setVisible(true);
			logo->setVisible(true);
			world.markForDelete(this);
		}
	}
}

State Game::getCurrentState(){
	return this->state;
}

void Game::setCurrentState(State value){
	this->state = value;
}

void Game::draw(){
	GraphicsManager& g = GraphicsManager::getInstance();
	tutorial* t = &this->GameState.TutorialState;
	g.drawString(Position(t->stringX, t->stringY), "Hello World.", Justification::LEFT_JUSTIFIED, 4);
}
