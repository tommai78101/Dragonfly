#include "..\header\Exit.h"

Exit::Exit(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	setType(TYPE_EXIT);
	setTransparency();
	setSolidness(Solidness::HARD);

	Position pos(GameState->Stage1.exit.x + GameState->Bounds.minX + 1, GameState->Stage1.exit.y + GameState->Bounds.minY + 1);
	setPosition(pos);
	l.writeLog("[Exit] New position: %d %d", pos.getX(), pos.getY());

	registerInterest(DF_STEP_EVENT);
	
	setVisible(true);
	l.writeLog("[Exit] Successfully loaded Exit.");
}

Exit::~Exit(){
	unregisterInterest(DF_STEP_EVENT);
}

int Exit::eventHandler(Event* e){
	return 1;
}

void Exit::draw(){
	GraphicsManager& g = GraphicsManager::getInstance();
	Position pos = this->getPosition();
	g.drawCh(pos, '%', 5);
}