#include "..\header\Block.h"

Block::Block(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();

	Assert(l.isStarted() && w.isStarted());

	setType(TYPE_BLOCK);
	setTransparency();
	setSolidness(Solidness::HARD);

	//TODO(Thompson): Add blocks in each stage. Must not have overlapping data in the game state.
	block_state* blocks = GameState->Stage1.blocks;
	for (int i = 0; i < ArrayLength(blocks); i++){
		Position pos(blocks[i].initialX, blocks[i].initialY);
		setPosition(pos);
	}

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
	g.drawCh(pos, 'O', 7);
	return;
}