#include "..\header\Game.h"
#include "..\header\Menu.h"
#include "..\header\Player.h"
#include "..\header\Border.h"

Game::Game(Menu* menu){
	registerInterest(DF_STEP_EVENT);
	registerInterest(DF_KEYBOARD_EVENT);
	this->menu = menu;
	menu->unregisterInterest(DF_KEYBOARD_EVENT);

	initializeGameState();
}

void Game::initializeGameState(){
	LogManager& l = LogManager::getInstance();

	this->GameState = {};
	this->GameState.PlayerState.x = 40;
	this->GameState.PlayerState.y = 16;

	this->setCurrentState(State::TUTORIAL);

	Player* player = 0;
	Border* border = 0;

	//TODO(Thompson): May need to do an inline or something sort.
	
	WorldManager& w = WorldManager::getInstance();
	ObjectList list = w.getAllObjects();
	for (ObjectListIterator i(&list); !i.isDone(); i.next()){
		Object* obj = i.currentObject();
		if (obj){
			if (obj->getType().compare(TYPE_PLAYER) == 0){
				player = dynamic_cast<Player*>(obj);
			}
			else if (obj->getType().compare(TYPE_BORDER) == 0){
				border = dynamic_cast<Border*>(obj);
			}
		}
	}

	if (player){
		player->setVisible(true);
		player->initializeState(&GameState);
	}
	else {
		player = new Player(&this->GameState);
	}

	if (border){
		border->setVisible(true);
	}
	else {
		border = new Border(&GameState);
	}

	l.writeLog("[Game] Setting player game boundaries.");
	Position pos = border->getPosition();
	int width = border->getWidth() / 2;
	int height = border->getHeight() / 2;
	player->setGameBounds(pos.getX() - width, pos.getY() - height, pos.getX() + width-1, pos.getY() + height-1);
	
	l.writeLog("[Game] Finished initializing/resetting game state.");

	border = 0;
	player = 0;
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
			this->menu->reset();
			this->GameState.PlayerState.x = 40;
			this->GameState.PlayerState.y = 16;
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
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();

	stage* Stage = &this->GameState.Stage1;
	if (!Stage->layout){
		Stage->layout = (int*) new int[286] {
			0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,     0, 0,
			2, 9, 3, 8, 5,    7, 6, 5, 8, 9,    4, 5, 6, 9, 7,    8, 5, 4, 8, 9,     5, 6,
			0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,     0, 0,
			0, 1, 1, 1, 1,    0, 0, 0, 0, 0,    0, 0, 0, 1, 1,    1, 1, 1, 1, 1,     1, 1,
			0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,     0, 0,

			0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,     0, 0,
			0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,     0, 0,
			0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,     0, 0,
			0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,     0, 0,
			0, 0, 0, 0, 1,    0, 0, 0, 1, 1,    1, 1, 1, 1, 1,    1, 1, 1, 0, 0,     0, 0,

			0, 0, 0, 0, 1,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 1, 0, 0,     0, 0,
			0, 0, 0, 0, 1,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 1, 0, 0,     0, 0,
			0, 0, 0, 0, 1,    0, 0, 0, 0, 0,    0, 0, 0, 0, 0,    0, 0, 1, 0, 0,     0, 9
		};
		Stage->size = 13 * 22;
		Stage->width = 22;
		Stage->height = 13;
	}

	Position posBegin = Position(this->GameState.PlayerState.minX, this->GameState.PlayerState.minY);
	for (int Row = 0; Row < 13; Row++){ 
		for (int Column = 0; Column < 22; Column++){
			int value = Stage->layout[Row * 22 + Column];
			switch (value){
				case 0:{
					break;
				}
				case 1:{
					g.drawCh(Position(posBegin.getX() + Column + 1, posBegin.getY() + Row + 1), '*', 3);
					break;
				}
			}
			continue;
		}
	}
}
