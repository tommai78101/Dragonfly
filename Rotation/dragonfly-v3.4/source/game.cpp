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

	this->isResetting = false;

	new Pause('p');

	l.writeLog("[Game] Initializing complete.");
}

Game::~Game(){
	unregisterInterest(DF_STEP_EVENT);
	unregisterInterest(DF_KEYBOARD_EVENT);
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
		board* Board = &this->GameState->Board;
		
		if (this->isResetting){
			return 0;
		}

		ResourceManager& r = ResourceManager::getInstance();
		GraphicsManager& g = GraphicsManager::getInstance();
		if (!Board->isRotating){
			if (this->GameState->win.win){
				if (!this->GameState->win.isGameWinCreated){
					new GameWin(this->GameState, this);
					this->GameState->win.isGameWinCreated = true;
				}
			}
			else if (Board->rotateCCW){
				Board->rotateCCW = false;
				Sprite* rccw = r.getSprite("rotate-ccw");
				if (rccw){
					setSprite(rccw);
					setSpriteIndex(0);
					setSpriteSlowdown(3);
					setPosition(Position(g.getHorizontal()/2, g.getVertical()/2));
					setVisible(true);
					Board->arrayOrder--;
					if (Board->arrayOrder < 0){
						Board->arrayOrder = 3;
					}
					Board->isRotating = true;
				}
				else {
					l.writeLog("[GAME] Error: Cannot get sprite 'rotate-ccw' from resource manager.");
				}
			}
			else if (Board->rotateCW){
				Board->rotateCW = false;
				Sprite* rcw = r.getSprite("rotate-cw");
				if (rcw){
					setSprite(rcw);
					setSpriteIndex(0);
					setSpriteSlowdown(3);
					setVisible(true);
					setPosition(Position(g.getHorizontal() / 2, g.getVertical() / 2));
					Board->arrayOrder++;
					if (Board->arrayOrder > 3){
						Board->arrayOrder = 0;
					}
					Board->isRotating = true;
				}
				else {
					l.writeLog("[GAME] Error: Cannot get sprite 'rotate-cw' from resource manager.");
				}
			}
		}
		else {
			int index = this->getSpriteIndex();
			if (this->getSprite()){
				if (index == this->getSprite()->getFrameCount()-1){
					Board->isRotating = false;
					Board->rotateCCW = false;
					Board->rotateCW = false;
				}
			}
			else {
				l.writeLog("[GAME] Error: No sprites set.");
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
			this->isResetting = true;
			reset();
			return 1;
		}
	}
	return 0;
}

void Game::reset(){
	if (GameState->Stage1.currentStageLevel < this->menu->getLevelState().maxStageLevel-1 && GameState->win.win){
		GameState->Stage1.previousStageLevel = GameState->Stage1.currentStageLevel;
		GameState->Stage1.currentStageLevel++;
		this->menu->nextStage();
	}
	else {
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
		this->menu->registerInterest(DF_KEYBOARD_EVENT);
		this->menu->reset();
		this->unregisterInterest(DF_STEP_EVENT);
	}
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

void Game::setGameResetFlag(bool value){
	this->isResetting = value;
	return;
}


