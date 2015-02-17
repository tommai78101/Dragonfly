#include "..\header\Common.h"

#include "..\header\Menu.h"
#include "..\header\Game.h"
#include "..\header\Player.h"
#include "..\header\Border.h"
#include "..\header\Block.h"
#include "..\header\Exit.h"
#include "..\header\GameWin.h"

std::string Info[] {
		"How to play:                ",
		"----------------------------",
		"Space - Confirm/Enter       ",
		"Q     - Rotate Left         ",
		"W     - Start Game          ",
		"E     - Rotate Right        ",
		"R     - Cancel/Reset/Back   ",
		"A     - Move Left           ",
		"S     - Quit Game           ",
		"D     - Move Right          ",
		"P     - Pause               ",
		"CTRL+C- Force quit          "
};

//Remember, w = h = 256, which is large.
//Our screen buffer w and h is around 256, 80 respectively.

//TODO(Thompson): Separate Logo class from Menu. It's now getting in the way.


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


Menu::Menu(){
	LogManager& log = LogManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	WorldManager& world = WorldManager::getInstance();

	if (!resource.isStarted() || !g.isStarted() || !world.isStarted()){
		log.writeLog("[Menu] Something is wrong with manager startups. Order: %s %s %s", 
		BoolToString(resource.isStarted()), BoolToString(g.isStarted()), BoolToString(world.isStarted()));
		world.markForDelete(this);
		return;
	}

	Sprite* tempSprite = resource.getSprite("square_spinning");
	if (tempSprite){
		log.writeLog("[Menu] Successfully loaded Menu sprite.");

		setSprite(tempSprite);
		setSpriteSlowdown(5);
		setTransparency();
		setType(TYPE_MENU);
		registerInterest(DF_STEP_EVENT);
		registerInterest(DF_KEYBOARD_EVENT);

		int w = g.getHorizontal()/2;
		int h = g.getVertical()/2;
		Position pos(w, h);
		setPosition(pos);

		setVisible(true);

		Menu::initalSpin = false;
		Menu::canSelectOptions = false;
		Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 4 + 2);
		Menu::StartGame = true;

		log.writeLog("[Menu] Finished loading Menu");
	}
	else {
		log.writeLog("[Menu] Sprite \"square_spinning\" not found.");
		world.markForDelete(this);
	}
}

Menu::~Menu(){
	delete[] this->GameState.Stage1.layout;
	delete[] this->GameState.Stage1.blocks;
	delete[] this->levels.stage1;
	delete[] this->levels.stage2;
	delete[] this->levels.stage3;
	delete[] this->levels.temp;
	this->unregisterInterest(DF_STEP_EVENT);
	this->unregisterInterest(DF_KEYBOARD_EVENT);
}

int Menu::eventHandler(Event* e){
	WorldManager& world = WorldManager::getInstance();
	LogManager& log = LogManager::getInstance();
	if (e->getType() == DF_STEP_EVENT){
		int SpriteIndex = this->getSpriteIndex();
		if (SpriteIndex == 0 && Menu::initalSpin){
			this->setSpriteIndex(4);
		}
		else if (SpriteIndex == 9 && !Menu::initalSpin){
			Menu::initalSpin = true;
			Menu::canSelectOptions = true;
			setSpriteSlowdown(3);
			GraphicsManager& g = GraphicsManager::getInstance();
			Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 4 + 2);
			new Logo(this->getPosition());
		}
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT && Menu::canSelectOptions){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		LogManager& log = LogManager::getInstance();
		GraphicsManager& g = GraphicsManager::getInstance();

		if (key == 'w'){
			Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 4 + 2);
			Menu::StartGame = true;
		}
		else if (key == 's'){
			Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 5);
			Menu::StartGame = false;
		}
		else if (key == ' '){
			WorldManager& world = WorldManager::getInstance();
			ObjectList list = world.getAllObjects();
			if (this->isVisible()){
				if (Menu::StartGame){
					this->setVisible(false);
					bool GameIsInWorld = false;
					Object* game = 0;
					for (ObjectListIterator i(&list); !i.isDone(); i.next()){
						Object* obj = i.currentObject();
						if (obj->getType().compare(TYPE_LOGO) == 0){
							obj->setVisible(false);
						}
						else if (obj->getType().compare(TYPE_GAME) == 0){
							game = obj;
							GameIsInWorld = true;
							Menu::StartGame = true;
						}
					}
				
					Game* gameObject = 0;
					if (!GameIsInWorld || !game){
						gameObject = new Game(this, &this->GameState);
					}
					else {
						gameObject = dynamic_cast<Game*>(game);
						gameObject->registerInterest(DF_STEP_EVENT);
					}
					gameObject->setVisible(true);
					gameObject->setGameResetFlag(false);
					this->initializeGameState();
					this->unregisterInterest(DF_KEYBOARD_EVENT);
				}
				else {
					for (ObjectListIterator i(&list); !i.isDone(); i.next()){
						Object* obj = i.currentObject();
						obj->setVisible(false);
						world.markForDelete(obj);
					}
					GameManager& game = GameManager::getInstance();
					game.setGameOver();
				}
			}
		}
		return 1;
	}
	return 0;
}

void Menu::draw(){
	Object::draw();
	if (Menu::canSelectOptions){
		GraphicsManager& g = GraphicsManager::getInstance();
		g.drawCh(Menu::cursorPosition, '>', 2);

		Position text = Position(Menu::cursorPosition.getX() + 6, (g.getVertical() / 6) * 4 + 2);
		g.drawString(text, "Start Game", Justification::LEFT_JUSTIFIED, 2);

		text.setY((g.getVertical() / 6) * 5);
		g.drawString(text, "Quit Game", Justification::LEFT_JUSTIFIED, 2);

		int y = 1;
		for (int i = 0; i < ArrayLength(Info); i++, y++){
			g.drawString(Position(1, y), Info[i], Justification::LEFT_JUSTIFIED, 4);
		}
	}
}

Logo::Logo(Position pos){
	LogManager& log = LogManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	WorldManager& world = WorldManager::getInstance();
	if (!resource.isStarted() || !g.isStarted() || !world.isStarted()){
		log.writeLog("[Logo] Something is wrong with manager startups. Order: %s %s %s", BoolToString(resource.isStarted()), BoolToString(g.isStarted()), BoolToString(world.isStarted()));
		world.markForDelete(this);
		return;
	}

	Sprite* tempSprite = resource.getSprite("logo");
	if (tempSprite){
		log.writeLog("[LOGO] Successfully loaded sprite.");
		setSprite(tempSprite);
		setSpriteSlowdown(1);
		setType(TYPE_LOGO);
		setTransparency();

		registerInterest(DF_STEP_EVENT);
		registerInterest(DF_KEYBOARD_EVENT);

		int w = g.getHorizontal() / 2;
		int h = g.getVertical() / 2;
		Position pos(w, h);
		setPosition(pos);

		setVisible(true);

		Logo::goUp = 0;
		log.writeLog("[LOGO] Finished initializing object.");
	}
	else {
		log.writeLog("[Logo] Sprite \"square_spinning\" not found.");
	}
}

int Logo::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT){
		return 1;
	}
	return 0;
}

void Logo::draw(){
	Object::draw();
}

void Menu::reset(){
	Menu::initalSpin = false;
	Menu::canSelectOptions = false;
	GraphicsManager& g = GraphicsManager::getInstance();
	Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 4 + 2);
	Menu::StartGame = true;

	this->GameState.Board = {};

	this->registerInterest(DF_KEYBOARD_EVENT);
}


void Menu::initializeGameState(){
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

	initializeLevels();

	if (border){
		border->setVisible(true);
	}
	else {
		border = new Border(&this->GameState);
	}

	l.writeLog("[Menu] Setting player game boundaries.");
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
	nextStage();
	Assert(*(Stage->layout + 4) >= 0);
	Stage->blockStateSize = 0;
	for (int i = 0; i < Stage->size; i++){
		if (Stage->layout[i] == 2){
			Stage->blockStateSize++;
		}
	}
	Stage->blocks = new block_state[Stage->blockStateSize];

	for (int i = 0, j = 0; i < Stage->size; i++){
		switch (Stage->layout[i]){
			case 2: {
				if (j < Stage->blockStateSize){
					Stage->blocks[j].initialX = (i % Stage->width);
					Stage->blocks[j].initialY = (i / Stage->width);
					Stage->blocks[j].x = Stage->blocks[j].initialX;
					Stage->blocks[j].y = Stage->blocks[j].initialY;
					int k = Stage->blocks[j].initialX;
					int n = Stage->blocks[j].initialY;
					j++;
				}
				else {
					l.writeLog("[Game] Error: Too many blocks placed in layout. Increase block state size or remove excess blocks.");
				}
				break;
			}
			case 8: {
				this->GameState.PlayerState = {};
				this->GameState.PlayerState.initialX = (i % Stage->width) + this->GameState.Bounds.minX;
				this->GameState.PlayerState.initialY = (i / Stage->width) + this->GameState.Bounds.minY;
				this->GameState.PlayerState.x = this->GameState.PlayerState.initialX;
				this->GameState.PlayerState.y = this->GameState.PlayerState.initialY;
				break;
			}
		}
	}

	if (player){
		player->setVisible(true);
		player->initializeState(&this->GameState);
	}
	else {
		player = new Player(&this->GameState);
	}

	this->GameState.win = {};
	this->GameState.win.win = false;
	this->GameState.win.isGameWinCreated = false;

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

	for (int i = 0; i < Stage->blockStateSize; i++){
		new Block(&this->GameState, i);
	}

	new Exit(&this->GameState);

	l.writeLog("[Menu] Finished initializing/resetting game state.");

	border = 0;
	player = 0;
}

void Menu::initializeLevels(){
	if (!this->isLevelInitialized){
		int size = 13 * 13;
		this->levels = {};
		this->levels.size = size;
		this->levels.stage1 = new int[size] {
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
		this->levels.stage2 = new int[size] {
			0,0,0,0,0,0,0,0,0,8,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,0,0,0,0,0,0,0,1,1,1,
			0,0,0,0,0,0,0,0,0,0,0,0,9,
			0,0,0,0,0,0,2,0,0,0,0,1,1,
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,1,1,1,1,0,0,0,0,0,0,
			0,0,0,1,0,0,0,0,0,0,0,0,0,
			0,0,0,1,0,0,0,0,0,0,2,0,0,
			0,0,0,1,0,0,0,0,0,0,0,0,0,
			0,0,0,1,0,0,0,0,0,0,0,0,0,
			0,1,0,0,0,0,0,0,0,0,0,1,0
		};
		this->levels.stage3 = new int[size]{
			0,0,0,1,0,0,0,0,0,0,0,0,0,
			0,2,0,1,0,0,0,0,0,0,0,0,0,
			0,0,0,1,0,0,0,0,0,0,0,0,0,
			1,1,1,1,0,0,0,0,0,0,0,0,1,
			0,0,0,0,0,0,0,0,0,0,0,0,9,
			0,0,0,0,0,0,0,0,0,0,0,0,1,
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			8,0,0,0,0,0,0,0,0,0,0,0,0,
			2,2,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,1,1,1,0,
			1,1,1,1,1,0,0,0,0,1,0,0,0,
			0,2,2,0,1,0,0,0,0,1,0,0,0,
			0,0,0,0,1,0,0,0,0,1,0,0,0
		};
		this->levels.temp = new int[size];
		this->levels.maxStageLevel = 3;
		this->isLevelInitialized = true;
	}
	else {
		if (this->levels.currentLevel < this->levels.maxStageLevel){
			this->levels.currentLevel++;
			this->GameState.Stage1.currentStageLevel = this->levels.currentLevel;
		}
		else {
			WorldManager& w = WorldManager::getInstance();
			ObjectList l = w.getAllObjects();
			for (ObjectListIterator i(&l); !i.isDone(); i.next()){
				Object* o = i.currentObject();
				if (o->getType().compare(TYPE_GAME) == 0){
					dynamic_cast<Game*>(o)->reset();
					break;
				}
			}
			this->reset();
		}
	}
	return;
}

void Menu::nextStage(){
	switch (this->GameState.Stage1.currentStageLevel){
		default:{
			for (int i = 0; i < this->levels.size; i++){
				this->levels.temp[i] = this->levels.stage1[i];
			}
			break;		   
		}				   
		case 1: {		   
			for (int i = 0; i < this->levels.size; i++){
				this->levels.temp[i] = this->levels.stage2[i];
			}
			break;		   
		}				   
		case 2: {		   
			for (int i = 0; i < this->levels.size; i++){
				this->levels.temp[i] = this->levels.stage3[i];
			}
			break;
		}
	}
	this->GameState.Stage1.layout = this->levels.temp;
	this->GameState.Board = {};

	WorldManager& w = WorldManager::getInstance();
	ObjectList list = w.getAllObjects();
	Player* player = 0;
	for (ObjectListIterator i(&list); !i.isDone(); i.next()){
		Object* obj = i.currentObject();
		if (obj->getType().compare(TYPE_BLOCK) == 0){
			w.markForDelete(obj);
		}
		else if (obj->getType().compare(TYPE_PLAYER) == 0){
			player = dynamic_cast<Player*>(obj);
		}
	}
	delete[] this->GameState.Stage1.blocks;

	stage* Stage = &this->GameState.Stage1;
	Stage->blockStateSize = 0;
	for (int i = 0; i < Stage->size; i++){
		if (Stage->layout[i] == 2){
			Stage->blockStateSize++;
		}
	}
	Stage->blocks = new block_state[Stage->blockStateSize];

	for (int i = 0, j = 0; i < this->levels.size; i++){
		switch (this->levels.temp[i]){
			case 2: {
				if (j < Stage->blockStateSize){
					Stage->blocks[j].initialX = (i % Stage->width);
					Stage->blocks[j].initialY = (i / Stage->width);
					Stage->blocks[j].x = Stage->blocks[j].initialX;
					Stage->blocks[j].y = Stage->blocks[j].initialY;
					int k = Stage->blocks[j].initialX;
					int n = Stage->blocks[j].initialY;
					j++;
				}
				break;
			}
			case 8: {
				this->GameState.PlayerState = {};
				this->GameState.PlayerState.initialX = (i % Stage->width) + this->GameState.Bounds.minX;
				this->GameState.PlayerState.initialY = (i / Stage->width) + this->GameState.Bounds.minY;
				this->GameState.PlayerState.x = this->GameState.PlayerState.initialX;
				this->GameState.PlayerState.y = this->GameState.PlayerState.initialY;
				break;
			}
		}
	}

	if (player){
		player->setVisible(true);
		player->initializeState(&this->GameState);
		player = 0;
	}

	for (int i = 0; i<Stage->blockStateSize; i++){
		new Block(&this->GameState, i);
	}
	return;
}



levels Menu::getLevelState() const{
	return this->levels;
}



