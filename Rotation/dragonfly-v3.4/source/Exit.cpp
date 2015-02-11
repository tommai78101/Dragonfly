#include "..\header\Exit.h"

Exit::Exit(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	setType(TYPE_EXIT);
	setTransparency();
	setSolidness(Solidness::HARD);

	GameState->Stage1.exit.x += GameState->Bounds.minX;
	GameState->Stage1.exit.y += GameState->Bounds.minY;
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
	LogManager& l = LogManager::getInstance();
	if (e->getType().compare(DF_STEP_EVENT) == 0){
		block_state* Blocks = this->GameState->Stage1.blocks;
		this->GameState->Stage1.exit.isBlocked = false;
		for (int i = 0; i<this->GameState->Stage1.blockStateSize; i++){
			if (Blocks[i].x == this->GameState->Stage1.exit.x && Blocks[i].y == this->GameState->Stage1.exit.y){
				this->GameState->Stage1.exit.isBlocked = true;
			}
		}
		return 1;
	}
	return 0;
}

void Exit::draw(){
	if (this->GameState->Board.isRotating){
		return;
	}
	if (this->GameState->Stage1.exit.isBlocked){
		return;
	}

	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	switch (this->GameState->Board.arrayOrder){
		case 0:{
			Position pos(this->getPosition().getX()+1, this->getPosition().getY()+1);
			g.drawCh(pos, '%', 5);
			break;
		}
		case 1:{
			int x = this->GameState->Stage1.exit.x;
			int y = this->GameState->Stage1.exit.y;
			int newY = this->GameState->Bounds.minY + (x - this->GameState->Bounds.minX);
			int newX = this->GameState->Bounds.minX + ((this->GameState->Bounds.maxY-1) - y);
			Position newPos(newX + 1, newY + 1);
			g.drawCh(newPos, '%', 5);
			break;
		}
		case 2:{
			int x = this->GameState->Stage1.exit.x;
			int y = this->GameState->Stage1.exit.y;
			int newX = (this->GameState->Bounds.maxX-1) - (x - this->GameState->Bounds.minX);
			int newY = this->GameState->Bounds.minY + ((this->GameState->Bounds.maxY-1) - y);
			Position newPos(newX+1, newY+1);
			g.drawCh(newPos, '%', 5);
			break;
		}
		case 3:{
			int x = this->GameState->Stage1.exit.x;
			int y = this->GameState->Stage1.exit.y;
			int newX = (this->GameState->Bounds.maxX-1) - ((this->GameState->Bounds.maxY-1) - y);
			int newY = (this->GameState->Bounds.maxY-1) - (x - this->GameState->Bounds.minX);
			Position newPos(newX+1, newY+1);
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