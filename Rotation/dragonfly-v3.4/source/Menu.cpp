#include "..\header\Common.h"

#include "..\header\Menu.h"
#include "..\header\Game.h"

//Remember, w = h = 256, which is large.
//Our screen buffer w and h is around 256, 80 respectively.

Menu::Menu(){
	LogManager& log = LogManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	WorldManager& world = WorldManager::getInstance();

	Sprite* tempSprite = resource.getSprite("square_spinning");
	if (tempSprite){
		log.writeLog("Successfully loaded sprite.");

		setSprite(tempSprite);
		setSpriteSlowdown(5);
		setTransparency();
		setLocation(CENTER_CENTER);
		setType(TYPE_MENU);
		registerInterest(DF_STEP_EVENT);
		registerInterest(DF_KEYBOARD_EVENT);

		int w = g.getHorizontal()/2;
		int h = g.getVertical()/2;
		world.moveObject(this, Position(w, h));

		setVisible(true);

		Menu::initalSpin = false;
		Menu::canSelectOptions = false;
		Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 4);
		Menu::StartGame = true;
	}
	else {
		log.writeLog("Menu::Menu(): Sprite \"square_spinning\" not found.");
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
			Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 4);
			new Logo;
		}
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT && Menu::canSelectOptions){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		LogManager& log = LogManager::getInstance();
		GraphicsManager& g = GraphicsManager::getInstance();

		//TODO(Thompson): Need to add control instructions for the user to see what to press.
		if (key == 'w'){
			Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 4);
			Menu::StartGame = true;
		}
		else if (key == 's'){
			Menu::cursorPosition = Position(g.getHorizontal() / 4 + 2, (g.getVertical() / 6) * 5);
			Menu::StartGame = false;
		}
		else if (key == ' '){
			WorldManager& world = WorldManager::getInstance();
			ObjectList list = world.getAllObjects();
			if (Menu::StartGame){
				this->setVisible(false);
				for (ObjectListIterator i(&list); !i.isDone(); i.next()){
					Object* obj = i.currentObject();
					if (obj->getType().compare(TYPE_LOGO) == 0){
						obj->setVisible(false);
					}
				}
				
				//TODO: Start game here.
				//TODO: Need another object to activate this when it's no longer active.

				new Game;
			}
			else {
				for (ObjectListIterator i(&list); !i.isDone(); i.next()){
					Object* obj = i.currentObject();
					obj->setVisible(false);
					world.markForDelete(obj);
				}
			}
		}
		else if (key == 'r'){
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

		Position text = Position(Menu::cursorPosition.getX() + 6, (g.getVertical() / 6) * 4);
		g.drawString(text, "Start Game", Justification::LEFT_JUSTIFIED, 2);

		text.setY((g.getVertical() / 6) * 5);
		g.drawString(text, "Quit Game", Justification::LEFT_JUSTIFIED, 2);

	}
}

//TODO(Thompson): Find a way to mark the Logo for deletion when the user clicks on "Start Game" or "Quit Game".
Logo::Logo(){
	setType(TYPE_LOGO);
	LogManager& log = LogManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();

	Sprite* tempSprite = resource.getSprite("logo");
	if (tempSprite){
		log.writeLog("[LOGO] Successfully loaded sprite.");

		setSprite(tempSprite);
		setSpriteSlowdown(15);
		setTransparency();
		setLocation(CENTER_CENTER);
		registerInterest(DF_STEP_EVENT);

		int w = g.getHorizontal() / 2;
		int h = g.getVertical() / 2;
		WorldManager& world = WorldManager::getInstance();
		world.moveObject(this, Position(w, h));

		setVisible(true);
	}
	else {
		log.writeLog("Menu::Menu(): Sprite \"square_spinning\" not found.");
	}
	Logo::goUp = 0;
}

int Logo::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		if (Logo::goUp > 6){
			getPosition().setXY(getPosition().getX(), getPosition().getY()-1);
		}
		else if (Logo::goUp > 3) {
			getPosition().setXY(getPosition().getX(), getPosition().getY()+1);
		}
		Logo::goUp++;
		if (Logo::goUp > 9){
			Logo::goUp = 0;
		}
	}
}

void Logo::draw(){
	Object::draw();
}