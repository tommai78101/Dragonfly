#include "..\header\Block.h"

bool checkPlayerCollision(game_state* GameState, int blockX, int blockY, int blockID){
	bool result = true;
	if (GameState->PlayerState.x == blockX && GameState->PlayerState.y == blockY){
		result = false;
	}
	return result;
}

bool checkBlockCollision(game_state* GameState, int blockX, int blockY, int blockID){
	//blockX, blockY represents the block that is currently being updated on.
	//Any blocks from GameState must be treated as different from this block being worked on.
	//If block is the same as this block, continue.

	bool result = true;

	block_state* Blocks = GameState->Stage1.blocks;
	for (int i = 0; i < GameState->Stage1.blockStateSize; i++){
		if (Blocks[i].id != blockID){
			if (Blocks[i].x == blockX && Blocks[i].y == blockY){
				result = false;
				break;
			}
		}
	}

	return result;
}

bool checkCollision(game_state* GameState, int blockX, int blockY, int blockID){
	bool result = true;
	result = (checkPlayerCollision(GameState, blockX, blockY, blockID) && checkBlockCollision(GameState, blockX, blockY, blockID));
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
	blocks[id].id = id;
	setPosition(pos);

	registerInterest(DF_STEP_EVENT);
	setVisible(true);

	this->GameState = GameState;

	l.writeLog("[Block %d] Successfully loaded blocks.", id);
}

int Block::eventHandler(Event* e){
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	if (e->getType() == DF_STEP_EVENT){
		if (!this->GameState->Board.isRotating){
			Position pos = this->getPosition();
			int x = pos.getX();
			int y = pos.getY();
			int layoutX = (x - this->GameState->Bounds.minX);
			int layoutY = (y - this->GameState->Bounds.minY);
			int* layout = this->GameState->Stage1.layout;
			int width = this->GameState->Stage1.width;
			int height = this->GameState->Stage1.height;

			if (this->GameState && this->GameState->Stage1.layout){
				switch (this->GameState->Board.arrayOrder){
					case 0:{
						if (layoutY + 1 < height){
							if (layout[(layoutY + 1) * width + layoutX] == 0 && checkCollision(this->GameState, x, y+1, this->getBlockID())){
								y++;
							}
						}
						break;
					}
					case 1:{
						if (layoutX + 1 < width){
							if (layout[layoutY * width + (layoutX + 1)] == 0 && checkCollision(this->GameState, x + 1, y, this->getBlockID())){
								x++;
							}
						}
						break;
					}
					case 2:{
						if (layoutY - 1 >= 0){
							if (layout[(layoutY - 1)* width + layoutX] == 0 && checkCollision(this->GameState, x, y - 1, this->getBlockID())){
								y--;
							}
						}
						break;
					}
					case 3:{
						if (layoutX - 1 >= 0){
							if (layout[layoutY*width + (layoutX - 1)] == 0 && checkCollision(this->GameState, x - 1, y, this->getBlockID())){
								x--;
							}
						}
						break;
					}
				}
			}

			if (x < this->GameState->Bounds.minX){
				x = this->GameState->Bounds.minX;
			}
			if (y < this->GameState->Bounds.minY){
				y = this->GameState->Bounds.minY;
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

	//Position pos(this->getPosition().getX(), this->getPosition().getY());
	block_state* BlockState = &(this->GameState->Stage1.blocks[this->getBlockID()]);

	switch (this->GameState->Board.arrayOrder){
		case 0:{
			Position pos(this->getPosition().getX() + 1, this->getPosition().getY() + 1);
			g.drawCh(pos, 'O', 7);
			break;
		}
		case 1:{
			int x = BlockState->x;
			int newY = this->GameState->Bounds.minY + (x - this->GameState->Bounds.minX);
			int y = BlockState->y;
			int newX = this->GameState->Bounds.minX + (this->GameState->Bounds.maxY - y - 1);
			Position newPos(newX + 1, newY + 1);
			g.drawCh(newPos, 'O', 7);
			break;
		}
		case 2:{
			int x = BlockState->x;
			int newX = (this->GameState->Bounds.maxX - 1) - (x - this->GameState->Bounds.minX);
			int y = BlockState->y;
			int newY = this->GameState->Bounds.minY + ((this->GameState->Bounds.maxY - 1) - y);
			Position newPos(newX + 1, newY + 1);
			g.drawCh(newPos, 'O', 7);
			break;
		}
		case 3:{
			int x = BlockState->x;
			int newY = (this->GameState->Bounds.maxY-1) - (x - this->GameState->Bounds.minX);
			int y = BlockState->y;
			int newX = (this->GameState->Bounds.maxX-1) - ((this->GameState->Bounds.maxY-1) - y);
			Position newPos(newX+1, newY+1);
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