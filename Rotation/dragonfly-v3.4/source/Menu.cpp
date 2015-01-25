#include "..\header\Common.h"

#include "..\header\Menu.h"
#include <iostream>
#include <sstream>

//Remember, w = h = 256, which is large.
//Our screen buffer w and h is around 256, 80 respectively.

Menu::Menu(){
	setType(DF_TYPE_MENU);
	LogManager& log = LogManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();

	Sprite* tempSprite = resource.getSprite("square_spinning");
	if (tempSprite){
		log.writeLog("Successfully loaded sprite.");

		setSprite(tempSprite);
		setSpriteSlowdown(5);
		setTransparency();
		setLocation(CENTER_CENTER);
		registerInterest(DF_STEP_EVENT);
		registerInterest(DF_KEYBOARD_EVENT);

		int w = g.getHorizontal()/2;
		int h = g.getVertical()/2;
		WorldManager& world = WorldManager::getInstance();
		world.moveObject(this, Position(w, h));

		setActive(true);
	}
	else {
		log.writeLog("Menu::Menu(): Sprite \"square_spinning\" not found.");
	}

	Menu::initalSpin = false;
	Menu::cursorPosition = Position(g.getHorizontal()/4, (g.getVertical()/6)*4);
}

int Menu::eventHandler(Event* e){
	WorldManager& world = WorldManager::getInstance();
	LogManager& log = LogManager::getInstance();
	if (e->getType() == DF_STEP_EVENT){
		int SpriteIndex = this->getSpriteIndex();
		if (SpriteIndex == 0 && Menu::initalSpin){
			this->setSpriteIndex(4);
		}
		else if (SpriteIndex >= 9){
			Menu::initalSpin = true;
			setSpriteSlowdown(3);
		}
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		LogManager& log = LogManager::getInstance();
		GraphicsManager& g = GraphicsManager::getInstance();
		log.writeLog("Key pressed.");

		if (key == 'w'){
			Menu::cursorPosition = Position(g.getHorizontal() / 4, (g.getVertical() / 6) * 4);
		}
		else if (key == 's'){
			Menu::cursorPosition = Position(g.getHorizontal() / 4, (g.getVertical() / 6) * 5);
		}
	}
	return 0;
}

void Menu::draw(){
	Object::draw();
	GraphicsManager& g = GraphicsManager::getInstance();
	g.drawCh(Menu::cursorPosition, '>', 2);
}