#include "..\header\Common.h"

#include "..\header\Menu.h"
#include "..\header\Game.h"


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
		"CTRL+C- Force quit          "
};

//Remember, w = h = 256, which is large.
//Our screen buffer w and h is around 256, 80 respectively.

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
		//setLocation(CENTER_CENTER);
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
	this->unregisterInterest(DF_STEP_EVENT);
	this->unregisterInterest(DF_KEYBOARD_EVENT);

	GameManager& game = GameManager::getInstance();
	game.setGameOver();
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
				
					if (!GameIsInWorld || !game){
						new Game(this);
					}
					else {
						Game* gameObject = dynamic_cast<Game*>(game);
						gameObject->setVisible(true);
						gameObject->initializeGameState();
					}
				}
				else {
					for (ObjectListIterator i(&list); !i.isDone(); i.next()){
						Object* obj = i.currentObject();
						obj->setVisible(false);
						world.markForDelete(obj);
					}
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

//TODO(Thompson): Find a way to mark the Logo for deletion when the user clicks on "Start Game" or "Quit Game".
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
	this->StartGame = true;
	this->registerInterest(DF_KEYBOARD_EVENT);
}
