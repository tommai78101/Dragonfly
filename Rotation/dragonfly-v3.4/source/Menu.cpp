#include "..\header\Common.h"
#include "..\include\ViewObject.h"
#include "..\include\ObjectList.h"
#include "..\include\ObjectListIterator.h"

#include "..\header\Menu.h"

Menu::Menu(){
	setType(DF_TYPE_MENU);
	LogManager& log = LogManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	Sprite* tempSprite = resource.getSprite("square_spinning");
	if (tempSprite){
		log.writeLog("Successfully loaded sprite.");

		setSprite(tempSprite);
		setSpriteSlowdown(3);
		setLocation(CENTER_CENTER);
		registerInterest(DF_STEP_EVENT);
		registerInterest(DF_KEYBOARD_EVENT);
		setActive(true);
	}
	else {
		log.writeLog("Menu::Menu(): Sprite \"square_spinning\" not found.");
	}
}

int Menu::eventHandler(Event* e){
	WorldManager& world = WorldManager::getInstance();
	LogManager& log = LogManager::getInstance();
	if (e->getType() == DF_STEP_EVENT){
		Position tempPos = this->getPosition();
		log.writeLog("Step Event: Square Position: %d, %d", tempPos.getX(), tempPos.getY());

		int count = 0;
		ObjectList list = world.getAllObjects();
		for (ObjectListIterator i(&list); !i.isDone(); i.next()){
			count++;
		}
		log.writeLog("Total number of Objects: %d", count);
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