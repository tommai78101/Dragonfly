#include "..\header\Exit.h"

Exit::Exit(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	setType(TYPE_EXIT);
	setTransparency();
	setSolidness(Solidness::HARD);

	GameState->Stage1.exit.x += GameState->Bounds.minX + 1;
	GameState->Stage1.exit.y += GameState->Bounds.minY + 1;
	Position pos(GameState->Stage1.exit.x, GameState->Stage1.exit.y);
	setPosition(pos);
	l.writeLog("[Exit] New position: %d %d", pos.getX(), pos.getY());

	registerInterest(DF_STEP_EVENT);
	
	setVisible(true);

	this->GameState = GameState;
	l.writeLog("[Exit] Successfully loaded Exit.");
}

Exit::~Exit(){
	unregisterInterest(DF_STEP_EVENT);
}

int Exit::eventHandler(Event* e){
	return 1;
}

void Exit::draw(){
	if (this->GameState->Board.isRotating){
		return;
	}
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	Position pos = this->getPosition();
	switch (this->GameState->Board.arrayOrder){
		case 0:{
			g.drawCh(pos, '%', 5);
			break;
		}
		case 1:{
			int x = this->GameState->Stage1.exit.x;
			int y = this->GameState->Stage1.exit.y;
			int newY = this->GameState->Bounds.minY + (x - this->GameState->Bounds.minX);
			int newX = this->GameState->Bounds.minX + (this->GameState->Bounds.maxY - y + 1);
			Position newPos(newX, newY);
			g.drawCh(newPos, '%', 5);
			break;
		}
		case 2:{
			int x = this->GameState->Stage1.exit.x;
			int y = this->GameState->Stage1.exit.y;
			int newX = this->GameState->Bounds.maxX - (x - this->GameState->Bounds.minX - 1);
			int newY = this->GameState->Bounds.minY + (this->GameState->Bounds.maxY - y + 1);
			Position newPos(newX, newY);
			g.drawCh(newPos, '%', 5);
			break;
		}
		case 3:{
			int x = this->GameState->Stage1.exit.x;
			int y = this->GameState->Stage1.exit.y;
			int newX = this->GameState->Bounds.maxX - (this->GameState->Bounds.maxY - y);
			int newY = this->GameState->Bounds.maxY - (x - this->GameState->Bounds.minX - 1);
			Position newPos(newX, newY);
			g.drawCh(newPos, '%', 5);
			break;
		}
		default:{
			l.writeLog("[Exit] Wrong array order: %d", (int) (this->GameState->Board.arrayOrder));
			break;
		}
	}
	return;
}