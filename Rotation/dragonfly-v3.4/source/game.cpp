#include "..\header\Game.h"
#include "..\header\Menu.h"
#include "..\header\Player.h"
#include "..\header\Border.h"
#include "..\header\Block.h"
#include "..\header\Exit.h"
#include "..\header\GameWin.h"

void setGameBounds(game_state* GameState, int x, int y, int w, int h){
	if (GameState){
		GameState->Bounds.minX = x;
		GameState->Bounds.minY = y;
		GameState->Bounds.maxX = w;
		GameState->Bounds.maxY = h;
		LogManager& l = LogManager::getInstance();
		l.writeLog("[Game] Setting game boundaries for left, top, width, and height: %d, %d, %d, %d", x, y, w, h);
	}
}

Game::Game(Menu* menu){
	registerInterest(DF_STEP_EVENT);
	registerInterest(DF_KEYBOARD_EVENT);
	this->menu = menu;
	menu->unregisterInterest(DF_KEYBOARD_EVENT);

	new Pause('p');

	initializeGameState();
}

Game::~Game(){
	delete[] this->GameState.Stage1.layout;
	delete[] this->GameState.Stage1.blocks;
}

void Game::initializeGameState(){
	LogManager& l = LogManager::getInstance();

	Player* player = 0;
	Border* border = 0;

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


	this->GameState = {};

	if (border){
		border->setVisible(true);
	}
	else {
		border = new Border(&GameState);
	}

	l.writeLog("[Game] Setting player game boundaries.");
	Position pos = border->getPosition();
	int borderWidth = border->getWidth() / 2;
	int borderHeight = border->getHeight() / 2;
	setGameBounds(&this->GameState, pos.getX() - borderWidth, pos.getY() - borderHeight, pos.getX() + borderWidth - 1, pos.getY() + borderHeight - 1);

	this->GameState.Board = {};
	this->GameState.Board.arrayOrder = 0;
	this->GameState.Board.isRotating = false;

	this->GameState.Stage1 = {};
	//NOTE(Thompson): We're going to go with 13x13 square.
	stage* Stage = &this->GameState.Stage1;
	Stage->width = 13;
	Stage->height = 13;
	Stage->size = Stage->height*Stage->width;
	Stage->blockStateSize = 3;
	Stage->blocks = new block_state[Stage->blockStateSize];
	if (!Stage->layout){
		Stage->layout = (int*) new int[Stage->size] { 
			1,1,1,0,0,0,0,0,0,1,0,0,0,
			1,2,0,0,0,0,0,0,0,1,0,0,0,
			1,0,0,0,0,0,0,0,0,1,0,0,0,
			0,0,0,0,0,0,0,0,0,1,1,1,1,
			0,0,0,0,0,0,0,1,0,0,0,0,9,
			0,0,0,0,0,0,2,1,0,0,0,0,0,
			0,0,0,0,0,0,1,1,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,
			0,0,0,0,8,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,2,0,0,
			1,1,1,1,1,0,0,0,0,0,0,0,0,
			0,0,0,0,1,0,0,0,0,0,0,1,1,
			0,0,0,0,1,0,0,0,0,0,0,1,1
		};
	}
	for (int i = 0, j = 0; i < Stage->size; i++){
		switch (Stage->layout[i]){
			case 2: {
				if (j < Stage->blockStateSize){
					Stage->blocks[j].initialX = (i % Stage->width) + 1;
					Stage->blocks[j].initialY = (i / Stage->width) + 1;
					Stage->blocks[j].x = Stage->blocks[j].initialX;
					Stage->blocks[j].y = Stage->blocks[j].initialY;
					Stage->layout[i] = 0;
					int k = Stage->blocks[j].initialX;
					int n = Stage->blocks[j].initialY;
					l.writeLog("Found case #2 - %d %d", k, n);
					j++;
				}
				else {
					l.writeLog("[Game] Error: Too many blocks placed in layout. Increase block state size or remove excess blocks.");
				}
				break;
			}
			case 8: {
				l.writeLog("Found case #8");
				this->GameState.PlayerState = {};
				this->GameState.PlayerState.initialX = (i % Stage->width) + this->GameState.Bounds.minX;
				this->GameState.PlayerState.initialY = (i / Stage->width) + this->GameState.Bounds.minY;
				this->GameState.PlayerState.x = this->GameState.PlayerState.initialX;
				this->GameState.PlayerState.y = this->GameState.PlayerState.initialY;
				Stage->layout[i] = 0;
				break;
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
	

	Stage->win = {};
	Stage->win.win = false;
	Stage->win.isGameWinCreated = false;

	Stage->exit = {};
	Stage->exit.isBlocked = false;
	bool exitCheck = false;
	for (int i = 0; i < Stage->size; i++){
		if (Stage->layout[i] == 9){
			int exitX = i % Stage->width;
			int exitY = i / Stage->width;
			Stage->exit.x = exitX;
			Stage->exit.y = exitY;
			exitCheck = true;
			break;
		}
	}
	Assert(exitCheck);

	this->setCurrentState(State::TUTORIAL);
	
	for (int i = 0; i < Stage->blockStateSize; i++){
		new Block(&this->GameState, i);
	}

	new Exit(&GameState);
	
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
		if (this->GameState.Stage1.win.win){
			if (!this->GameState.Stage1.win.isGameWinCreated){
				new GameWin(&this->GameState, this);
				this->GameState.Stage1.win.isGameWinCreated = true;
			}
		}
		else if (!this->GameState.Board.isRotating){
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
			if (this->GameState.Stage1.win.win){
				return 0;
			}
			reset();
		}
		return 1;
	}
	return 0;
}

void Game::reset(){
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
		if (Stage->layout){
			Position posBegin = Position(this->GameState.Bounds.minX, this->GameState.Bounds.minY);
			for (int Row = 0; Row < Stage->height; Row++){ 
				for (int Column = 0; Column < Stage->width; Column++){
					int value;
					switch (this->GameState.Board.arrayOrder){
						case 0:{
							value = Stage->layout[Row * Stage->width + Column];
							break;
						}
						case 1:{
							value = Stage->layout[((Stage->width - 1) - Column) * Stage->width + Row];
							break;
						}
						case 2: {
							value = Stage->layout[((Stage->height - 1) - Row)*Stage->width + ((Stage->width - 1) - Column)];
							break;
						}
						case 3:{
							value = Stage->layout[Column*Stage->width + ((Stage->height - 1) - Row)];
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

			int playerPosX = 0;
			int playerPosY = 0;
			for (; playerPosY < Stage->height; playerPosY++){
				for (; playerPosX < Stage->width; playerPosX++){
					if (Stage->layout[playerPosY * Stage->width + playerPosX] == 999){
						Stage->layout[playerPosY * Stage->width + playerPosX] = 0;
						break;
					}
				}
			}
			this->GameState.PlayerState.x = playerPosX;
			this->GameState.PlayerState.y = playerPosY;
		}
	}
}

Menu* Game::getMenu() const {
	return this->menu;
}
