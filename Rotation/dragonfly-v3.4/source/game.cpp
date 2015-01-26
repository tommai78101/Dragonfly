#include "..\header\Game.h"
#include "..\header\Menu.h"


Game::Game(){
	new Menu;
	setType(TYPE_GAME);
	setTransparency();
	registerInterest(DF_KEYBOARD_EVENT);
	setVisible(false);
}

int Game::eventHandler(Event* e){
	LogManager& log = LogManager::getInstance();
	if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		if (key == 'r'){
			WorldManager& world = WorldManager::getInstance();
			ObjectList list = world.getAllObjects();
			Object* menu;
			Object* logo;
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

void Game::draw(){}
