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

Game::~Game(){
	//Object::~Object();
	delete [] this->GameState.Stage1.layout;
}

void Game::initializeGameState(){
	LogManager& l = LogManager::getInstance();

	this->GameState = {};
	this->GameState.PlayerState.x = 40;
	this->GameState.PlayerState.y = 16;

	this->GameState.Stage1 = {};
	//NOTE(Thompson): We're going to go with 13x13 square.
	stage* Stage = &this->GameState.Stage1;
	Stage->width = 13;
	Stage->height = 13;
	Stage->size = Stage->height*Stage->width;
	if (!Stage->layout){
		Stage->layout = (int*) new int[Stage->size] { 
			0, 0, 0, 0, 0,	 0, 0, 0, 0, 1,		0, 0, 0,
			0, 0, 0, 0, 0,	 0, 0, 0, 0, 1,		0, 0, 0,
			0, 0, 0, 0, 0,	 0, 0, 0, 0, 1,		0, 0, 0,
			1, 1, 1, 1, 0,	 0, 0, 0, 0, 1,		0, 0, 0,
			0, 0, 0, 0, 0,	 0, 0, 0, 0, 0,		0, 0, 0,

			0, 0, 0, 0, 0,	 0, 0, 0, 0, 0,		0, 0, 0,
			0, 0, 0, 0, 0,	 0, 0, 0, 0, 0,		0, 0, 0,
			0, 0, 0, 0, 0,	 0, 0, 0, 0, 0,		0, 0, 0,
			0, 0, 0, 0, 0,	 0, 0, 0, 0, 0,		0, 0, 0,
			0, 0, 0, 1, 0,	 0, 0, 0, 0, 1,		1, 1, 1,

			0, 0, 0, 1, 0,	 0, 0, 0, 0, 0,		0, 0, 0,
			0, 0, 0, 1, 0,	 0, 0, 0, 0, 0,		0, 0, 0,
			0, 0, 0, 1, 0,	 0, 0, 0, 0, 0,		0, 0, 0,
		};
	}

	this->setCurrentState(State::TUTORIAL);

	this->GameState.Board = {};

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
	LogManager& l = LogManager::getInstance();
	if (e->getType() == DF_STEP_EVENT){
		ResourceManager& r = ResourceManager::getInstance();
		GraphicsManager& g = GraphicsManager::getInstance();
		if (!this->GameState.Board.isRotating){
			if (this->GameState.Board.rotateCCW){
				this->GameState.Board.rotateCCW = false;
				Sprite* rccw = r.getSprite("rotate-ccw");
				if (rccw){
					setSprite(rccw);
					setSpriteIndex(0);
					setSpriteSlowdown(3);
					setPosition(Position(g.getHorizontal()/2, g.getVertical()/2));
					this->GameState.Board.arrayOrder--;
					if (this->GameState.Board.arrayOrder < 0){
						this->GameState.Board.arrayOrder = 3;
					}
					this->GameState.Board.isRotating = true;
				}
			}
			else if (this->GameState.Board.rotateCW){
				this->GameState.Board.rotateCW = false;
				Sprite* rcw = r.getSprite("rotate-cw");
				if (rcw){
					setSprite(rcw);
					setSpriteIndex(0);
					setSpriteSlowdown(3);
					setPosition(Position(g.getHorizontal() / 2, g.getVertical() / 2));
					this->GameState.Board.arrayOrder++;
					if (this->GameState.Board.arrayOrder > 3){
						this->GameState.Board.arrayOrder = 0;
					}
					this->GameState.Board.isRotating = true;
				}
			}
		}
		else {
			int index = this->getSpriteIndex();
			if (index == this->getSprite()->getFrameCount()-1){
				this->GameState.Board.isRotating = false;
				this->GameState.Board.rotateCCW = false;
				this->GameState.Board.rotateCW = false;
			}
		}
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
	if (this->GameState.Board.isRotating){
		Object::draw();
	}
	else {
		LogManager& l = LogManager::getInstance();
		GraphicsManager& g = GraphicsManager::getInstance();

		stage* Stage = &this->GameState.Stage1;
		//TODO(Thompson): Fix this rotation issue caused by rectangular arrays.
		//float(286/square size)    := This is the ratio for pinpointing arrays to the screen.
		//Floor function. floor()   := Get a value that is within the array size.
		//Use the value obtained to draw on the screen.
		if (Stage->layout){
			Position posBegin = Position(this->GameState.PlayerState.minX, this->GameState.PlayerState.minY);

			for (int Column = 0; Column < Stage->width; Column++){
				for (int Row = 0; Row < Stage->height; Row++){ 
					int value;
					switch (this->GameState.Board.arrayOrder){
						case 0:{
							value = Stage->layout[Column * Stage->width + Row];
							break;
						}
						case 1:{
							value = Stage->layout[Row*Stage->width + ((Stage->width - 1) - Column)];
							break;
						}
						case 2: {
							value = Stage->layout[((Stage->width - 1) - Column)*Stage->width + ((Stage->height - 1) - Row)];
							break;
						}
						case 3:{
							value = Stage->layout[((Stage->height - 1) - Row) * Stage->width + Column];
							break;
						}
						default: {
							l.writeLog("[Game] Wrong value: %d", (int)(this->GameState.Board.arrayOrder));
							break;
						}
					}
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

	}
}
