#include "..\header\Block.h"

Block::Block(game_state* GameState, int id){
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();

	Assert(l.isStarted() && w.isStarted());

	setType(TYPE_BLOCK);
	setTransparency();
	setSolidness(Solidness::HARD);
	setBlockID(id);
	//TODO(Thompson): Add blocks in each stage. Must not have overlapping data in the game state.
	block_state* blocks = GameState->Stage1.blocks;
	Position pos(blocks[id].initialX + GameState->Bounds.minX, blocks[id].initialY + GameState->Bounds.minY);
	blocks[id].x = pos.getX();
	blocks[id].y = pos.getY();
	setPosition(pos);

	l.writeLog("[block init()] Block pos: %d %d", pos.getX(), pos.getY());

	registerInterest(DF_STEP_EVENT);
	setVisible(true);

	this->GameState = GameState;

	l.writeLog("[Block] Successfully loaded blocks.");
}

int Block::eventHandler(Event* e){
	if (e->getType() == DF_STEP_EVENT){
		if (!this->GameState->Board.isRotating){
			//TODO(Thompson): Do something to save the block position data.
			//This data will then be used to draw to the screen.
			//Must follow the coordinate system used by the Player object.


		}
		return 1;
	}
	return 0;
}

//TODO(Thompson): Do more testing on drawing the blocks. Requires strenuous testing.
void Block::draw(){
	if (this->GameState->Board.isRotating){
		return;
	}
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();

	Position pos = this->getPosition();
	//TODO(Thompson): Do something about falling after rotating the board here.
	g.drawCh(pos, 'O', 7);
	l.writeLog("Block pos: %d %d", pos.getX(), pos.getY());

	block_state* blocks = this->GameState->Stage1.blocks;
	blocks[this->getBlockID()].x = pos.getX();
	blocks[this->getBlockID()].y = pos.getY();
	return;
}

void Block::setBlockID(int value){
	this->id = value;
	return;
}

int Block::getBlockID() const {
	return this->id;
}