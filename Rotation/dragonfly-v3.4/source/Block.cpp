#include "..\header\Block.h"

bool checkPlayerCollision(game_state* GameState, int blockX, int blockY){
	bool result = true;
	if (GameState->PlayerState.x == blockX && GameState->PlayerState.y == blockY){
		result = false;
	}
	return result;
}

bool checkBlockCollision(game_state* GameState, int blockX, int blockY){
	//blockX, blockY represents the block that is currently being updated on.
	//Any blocks from GameState must be treated as different from this block being worked on.
	//If block is the same as this block, continue.

	bool result = true;
	return result;
}

bool checkCollision(game_state* GameState, int blockX, int blockY){
	bool result = true;
	result = (checkPlayerCollision(GameState, blockX, blockY) && checkBlockCollision(GameState, blockX, blockY));
	return result;
}


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
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	if (e->getType() == DF_STEP_EVENT){
		if (!this->GameState->Board.isRotating){
			Position pos = this->getPosition();
			int x = pos.getX();
			int y = pos.getY();
			int layoutX = (x - this->GameState->Bounds.minX - 1);
			int layoutY = (y - this->GameState->Bounds.minY - 1);
			int* layout = this->GameState->Stage1.layout;
			int width = this->GameState->Stage1.width;
			int height = this->GameState->Stage1.height;

			if (this->GameState && this->GameState->Stage1.layout){
				switch (this->GameState->Board.arrayOrder){
					case 0:{
						if (layoutY + 1 < height){
							if (layout[(layoutY + 1) * width + layoutX] == 0 && checkCollision(this->GameState, x, y+1)){
								y++;
							}
						}
						break;
					}
					case 1:{
						if (layoutX + 1 < width){
							if (layout[layoutY * width + (layoutX + 1)] == 0 && checkCollision(this->GameState, x+1, y)){
								x++;
							}
						}
						break;
					}
					case 2:{
						if (layoutY - 1 >= 0){
							if (layout[(layoutY - 1)* width + layoutX] == 0 && checkCollision(this->GameState, x, y-1)){
								y--;
							}
						}
						break;
					}
					case 3:{
						if (layoutX - 1 >= 0){
							if (layout[layoutY*width + (layoutX - 1)] == 0 && checkCollision(this->GameState, x-1, y)){
								x--;
							}
						}
						break;
					}
				}
			}

			if (x <= this->GameState->Bounds.minX){
				x = this->GameState->Bounds.minX + 1;
			}
			if (y <= this->GameState->Bounds.minY){
				y = this->GameState->Bounds.minY + 1;
			}
			if (x > this->GameState->Bounds.maxX){
				x = this->GameState->Bounds.maxX;
			}
			if (y > this->GameState->Bounds.maxY){
				y = this->GameState->Bounds.maxY;
			}

			this->GameState->Stage1.blocks[this->getBlockID()].x = x;
			this->GameState->Stage1.blocks[this->getBlockID()].y = y;
			this->setPosition(Position(x, y));
		}
		return 1;
	}
	return 0;
}

void Block::draw(){
	if (this->GameState->Board.isRotating){
		return;
	}
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();

	int width = this->GameState->Stage1.width;
	int height = this->GameState->Stage1.height;
	Position pos = this->getPosition();
	block_state* BlockState = &(this->GameState->Stage1.blocks[this->getBlockID()]);

	switch (this->GameState->Board.arrayOrder){
		case 0:{	
			g.drawCh(pos, 'O', 7);
			break;
		}
		case 1:{
			int x = BlockState->x;
			int newY = this->GameState->Bounds.minY + (x - this->GameState->Bounds.minX);
			int y = BlockState->y;
			int newX = this->GameState->Bounds.minX + (this->GameState->Bounds.maxY - y + 1);
			Position newPos(newX, newY);
			g.drawCh(newPos, 'O', 7);
			break;
		}
		case 2:{
			int x = BlockState->x;
			int newX = this->GameState->Bounds.maxX - (x - this->GameState->Bounds.minX - 1);
			int y = BlockState->y;
			int newY = this->GameState->Bounds.minY + (this->GameState->Bounds.maxY - y + 1);
			Position newPos(newX, newY);
			g.drawCh(newPos, 'O', 7);
			break;
		}
		case 3:{
			int x = BlockState->x;
			int newY = this->GameState->Bounds.maxY - (x - this->GameState->Bounds.minX - 1);
			int y = BlockState->y;
			int newX = this->GameState->Bounds.maxX - (this->GameState->Bounds.maxY - y);
			Position newPos(newX, newY);
			g.drawCh(newPos, 'O', 7);
			break;
		}
		default:{
			l.writeLog("[Block %d] Wrong array order: %d", this->getBlockID(), (int) (this->GameState->Board.arrayOrder));
			break;
		}
	}
	return;
}

void Block::setBlockID(int value){
	this->id = value;
	return;
}

int Block::getBlockID() const {
	return this->id;
}