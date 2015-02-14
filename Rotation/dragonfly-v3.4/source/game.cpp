#include "..\header\Game.h"
#include "..\header\Menu.h"
#include "..\header\Player.h"
#include "..\header\Border.h"
#include "..\header\Block.h"
#include "..\header\Exit.h"
#include "..\header\GameWin.h"

Game::Game(Menu* menu, game_state* GameState){
	LogManager& l = LogManager::getInstance();

	registerInterest(DF_STEP_EVENT);
	registerInterest(DF_KEYBOARD_EVENT);

	this->menu = menu;
	menu->unregisterInterest(DF_KEYBOARD_EVENT);

	this->GameState = GameState;

	new Pause('p');

	l.writeLog("[Game] Initializing complete.");
}

Game::~Game(){
	delete[] this->levels.stage1;
	delete[] this->levels.stage2;
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
		if (this->GameState->win.win){
			if (!this->GameState->win.isGameWinCreated){
				new GameWin(this->GameState, this);
				this->GameState->win.isGameWinCreated = true;
			}
		}
		else if (!this->GameState->Board.isRotating){
			if (this->GameState->Board.rotateCCW){
				this->GameState->Board.rotateCCW = false;
				Sprite* rccw = r.getSprite("rotate-ccw");
				if (rccw){
					setSprite(rccw);
					setSpriteIndex(0);
					setSpriteSlowdown(3);
					setPosition(Position(g.getHorizontal()/2, g.getVertical()/2));
					this->GameState->Board.arrayOrder--;
					if (this->GameState->Board.arrayOrder < 0){
						this->GameState->Board.arrayOrder = 3;
					}
					this->GameState->Board.isRotating = true;
				}
			}
			else if (this->GameState->Board.rotateCW){
				this->GameState->Board.rotateCW = false;
				Sprite* rcw = r.getSprite("rotate-cw");
				if (rcw){
					setSprite(rcw);
					setSpriteIndex(0);
					setSpriteSlowdown(3);
					setPosition(Position(g.getHorizontal() / 2, g.getVertical() / 2));
					this->GameState->Board.arrayOrder++;
					if (this->GameState->Board.arrayOrder > 3){
						this->GameState->Board.arrayOrder = 0;
					}
					this->GameState->Board.isRotating = true;
				}
			}
		}
		else {
			int index = this->getSpriteIndex();
			if (index == this->getSprite()->getFrameCount()-1){
				this->GameState->Board.isRotating = false;
				this->GameState->Board.rotateCCW = false;
				this->GameState->Board.rotateCW = false;
			}
		}
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		if (key == 'r'){
			if (this->GameState->win.win){
				return 0;
			}
			reset();
			return 1;
		}
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
	if (this->GameState->Board.isRotating){
		Object::draw();
	}
	else {
		LogManager& l = LogManager::getInstance();
		GraphicsManager& g = GraphicsManager::getInstance();

		stage* Stage = &this->GameState->Stage1;
		if (Stage->layout){
			Position posBegin = Position(this->GameState->Bounds.minX, this->GameState->Bounds.minY);
			for (int Row = 0; Row < Stage->height; Row++){ 
				for (int Column = 0; Column < Stage->width; Column++){
					int value;
					switch (this->GameState->Board.arrayOrder){
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
							l.writeLog("[Game] Wrong value: %d", (int)(this->GameState->Board.arrayOrder));
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
			this->GameState->PlayerState.x = playerPosX;
			this->GameState->PlayerState.y = playerPosY;
		}
	}
}

Menu* Game::getMenu() const {
	return this->menu;
}

void Game::initializeLevels(int size){
	this->setCurrentState(State::TUTORIAL);

	Assert(size);

	this->levels = {};
	this->levels.size = size;
	this->levels.stage1 = new int[size] {
		0,0,0,0,0,0,0,0,0,8,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,9,
		0,0,0,0,0,0,2,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,2,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0
	};
	this->levels.stage2 = new int[size] {
		0,0,0,1,0,0,0,0,0,0,0,0,0,
		0,2,0,1,0,0,0,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,0,0,0,
		1,1,1,1,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,9,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		8,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,0,0,0,0,0,0,0,0,
		0,2,2,0,1,0,0,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,0,0,0,0,0
	};

	return;
}