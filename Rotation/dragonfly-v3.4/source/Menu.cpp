#include "..\header\Common.h"
#include "..\include\ViewObject.h"
#include "..\include\ObjectList.h"
#include "..\include\ObjectListIterator.h"

#include "..\header\Menu.h"

//Remember, w = h = 256, which is large.
//Our screen buffer w and h is around 256, 80 respectively.

Menu::Menu(){
	setType(DF_TYPE_MENU);
	LogManager& log = LogManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	Sprite* tempSprite = resource.getSprite("square_spinning");
	if (tempSprite){
		log.writeLog("Successfully loaded sprite.");

		setSprite(tempSprite);
		setSpriteSlowdown(3);
		setTransparency();
		setLocation(CENTER_CENTER);
		registerInterest(DF_STEP_EVENT);
		registerInterest(DF_KEYBOARD_EVENT);

		WorldManager& world = WorldManager::getInstance();
		Box box = world.getBoundary();
		int w = box.getHorizontal();
		int h = box.getVertical();
		world.moveObject(this, Position(128, 40));

		setActive(true);
	}
	else {
		log.writeLog("Menu::Menu(): Sprite \"square_spinning\" not found.");
	}

	Menu::initalSpin = false;
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
		}
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		LogManager& log = LogManager::getInstance();
		log.writeLog("Key pressed.");
		Position tempPos = this->getPosition();
		bool test = false;
		if (key == 'w'){
			tempPos.setXY(tempPos.getX(), tempPos.getY() - 1);
			test = true;
		}
		else if (key == 's'){
			tempPos.setXY(tempPos.getX(), tempPos.getY() + 1);
			test = true;
		}
		else if (key == 'a'){
			tempPos.setXY(tempPos.getX()-1, tempPos.getY());
			test = true;
		}
		else if (key == 'd'){
			tempPos.setXY(tempPos.getX() + 1, tempPos.getY());
			test = true;
		}
		else if (key == 'n'){
			new Menu;
			world.markForDelete(this);
		}
		if (test){
			world.moveObject(this, tempPos);
			this->setPosition(tempPos);
			log.writeLog("Key pressed! Square Position: %d, %d", tempPos.getX(), tempPos.getY());
			log.writeLog("World Boundary: %d, %d", world.getBoundary().getHorizontal(), world.getBoundary().getVertical());
		}
	}
	return 0;
}

void Menu::draw(){
	Object::draw();
}