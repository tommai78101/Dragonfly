#include "..\header\Border.h"

Border::Border(){
	LogManager& log = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	WorldManager& world = WorldManager::getInstance();
	ResourceManager& resource = ResourceManager::getInstance();
	if (!resource.isStarted() || !g.isStarted() || !world.isStarted()){
		log.writeLog("Something is wrong with manager startups. Order: %s %s %s", BoolToString(resource.isStarted()), BoolToString(g.isStarted()), BoolToString(world.isStarted()));
		world.markForDelete(this);
		return;
	}

	Sprite* tempSprite = resource.getSprite("border");
	if (tempSprite){
		setSprite(tempSprite);
		setSpriteSlowdown(5);
		setTransparency();
		setSolidness(Solidness::HARD);
		setType(TYPE_BORDER);

		int w = g.getHorizontal() / 2;
		int h = g.getVertical() / 2;
		Position pos(w, h);
		setPosition(pos);

		registerInterest(DF_STEP_EVENT);

		setVisible(true);
	}
}

int Border::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		return 1;
	}
	return 0;
}

void Border::draw(){
	Object::draw();
}