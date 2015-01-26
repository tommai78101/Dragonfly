#include "..\header\Game.h"
#include "..\header\Menu.h"


Game::Game(){
	this->GameState = {};
	this->setCurrentState(State::TUTORIAL);

	GraphicsManager& g = GraphicsManager::getInstance();
	tutorial* Tutorial = &this->GameState.TutorialState;
	Tutorial->stringX = g.getHorizontal() / 4 + 2;
	Tutorial->stringY = g.getVertical() / 2;
	Tutorial->floatUp = false;

	registerInterest(DF_STEP_EVENT);
}

int Game::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		if (this->getCurrentState() == State::TUTORIAL){
			tutorial* Tutorial = &this->GameState.TutorialState;
			if (Tutorial->floatUp){
				Tutorial->stringY--;
			}
			else {
				Tutorial->stringY++;
			}
			Tutorial->counter++;

			if (Tutorial->counter > 3){
				Tutorial->floatUp = !Tutorial->floatUp;
				Tutorial->counter = 0;
			}
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
