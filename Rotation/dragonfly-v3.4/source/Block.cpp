#include "..\header\Block.h"

Block::Block(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	
	this->GameState = GameState;
}

int Block::eventHandler(Event* e){
	return 0;
}

void Block::draw(){
	return;
}