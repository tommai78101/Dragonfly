#include "..\header\Game.h"
#include "..\header\Menu.h"

Game::Game(){
	LogManager& log = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	WorldManager& world = WorldManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	if (!resource.isStarted() || !g.isStarted() || !world.isStarted()){
		log.writeLog("Something is wrong with manager startups. Order: %s %s %s", BoolToString(resource.isStarted()), BoolToString(g.isStarted()), BoolToString(world.isStarted()));
		world.markForDelete(this);
		return;
	}

	Sprite* tempSprite = resource.getSprite("test_box");
	if (tempSprite){
		setSprite(tempSprite);
		setSpriteSlowdown(5);
		setTransparency();
		setType(TYPE_GAME);

		int w = g.getHorizontal() / 2;
		int h = g.getVertical() / 2;
		Position pos(w, h);
		setPosition(pos);

		registerInterest(DF_STEP_EVENT);
		registerInterest(DF_KEYBOARD_EVENT);

		initializeGameState();
	}
}

void Game::initializeGameState(){
	this->GameState = {};
	this->setCurrentState(State::TUTORIAL);

	GraphicsManager& g = GraphicsManager::getInstance();
}

int Game::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		return 1;
	}
	else if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		if (key == 'r'){
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
			this->setVisible(false);
		}
		return 1;
	}
	return 0;
}

State Game::getCurrentState(){
	return this->state;
}

void Game::setCurrentState(State value){
	this->state = value;
}

void Game::draw(){
	Object::draw();
}
