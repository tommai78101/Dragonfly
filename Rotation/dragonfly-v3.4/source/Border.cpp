#include "..\header\Game.h"
#include "..\header\Border.h"

Border::Border(game_state* GameState){
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
		log.writeLog("Successfully loaded Border sprite.");
		setSprite(tempSprite);
		setSpriteSlowdown(5);
		setTransparency();
		setSolidness(Solidness::HARD);
		setType(TYPE_BORDER);
		registerInterest(DF_STEP_EVENT);
		setVisible(true);


		int w = g.getHorizontal() / 2;
		int h = g.getVertical() / 2;
		Position pos(w, h);
		setPosition(pos);

		this->width = tempSprite->getWidth();
		this->height = tempSprite->getHeight();
		GameState->PlayerState.minX = w - (tempSprite->getWidth() / 2);
		GameState->PlayerState.minX = h - (tempSprite->getHeight() / 2);
		GameState->PlayerState.maxX = w + (tempSprite->getWidth() / 2);
		GameState->PlayerState.maxY = h + (tempSprite->getHeight() / 2);
	}
	else {
		log.writeLog("Something is wrong with loading the sprite. Aborting.");
	}
	return;
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

int Border::getWidth(){
	return this->width;
}

int Border::getHeight(){
	return this->height;
}