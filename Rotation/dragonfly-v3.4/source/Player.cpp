#include "../header/Player.h"
#include "../header/GameWin.h"

static bool checkTile(int tileID){
	bool result;
	switch (tileID){
		case 0: {
			result = true;
			break;
		}
		case 1: {
			result = false;
			break;
		}
		case 2: {
			result = true;
			break;
		}
		case 8: {
			result = true;
			break;
		}
		case 9: {
			result = true;
			break;
		}
		default: {
			result = false;
			break;
		}
	}
	return result;
}

bool checkForExit(game_state* GameState, int playerX, int playerY){
	LogManager& l = LogManager::getInstance();
	if (GameState->Stage1.exit.isBlocked){
		return false;
	}
	bool result = false;
	if ((GameState->Stage1.exit.x == playerX) && (GameState->Stage1.exit.y == playerY)){
		GameState->win.win = true;
		result = true;
	}
	return result;
}

bool checkBlockCollision(game_state* GameState, int playerX, int playerY){
	bool result = true;
	block_state* blocks = GameState->Stage1.blocks;
	for (int i = 0; i<GameState->Stage1.blockStateSize; i++){
		if (blocks[i].x == playerX && blocks[i].y == playerY){
			result = false;
			break;
		}
	}
	return result;
}

Player::Player(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	Assert(l.isStarted() && w.isStarted());

	//No need for a sprite. A simple character will do.
	setType(TYPE_PLAYER);
	setTransparency();
	setSolidness(Solidness::HARD);
	Position pos(GameState->PlayerState.initialX, GameState->PlayerState.initialY);
	setPosition(pos);

	registerInterest(DF_KEYBOARD_EVENT);
	registerInterest(DF_STEP_EVENT);

	this->GameState = GameState;
	setVisible(true);
	l.writeLog("[Player] Successfully loaded Player entity.");
}

int Player::eventHandler(Event* e){
	LogManager& l = LogManager::getInstance();
	if (e->getType() == DF_KEYBOARD_EVENT){
		if (this->GameState->Board.isRotating){
			return 1;
		}
		if (this->GameState->win.win){
			return 1;
		}
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();

		int x = this->getPosition().getX();
		int y = this->getPosition().getY();
		int layoutX = (x - this->GameState->Bounds.minX);
		int layoutY = (y - this->GameState->Bounds.minY);
		int* layout = this->GameState->Stage1.layout;
		int width = this->GameState->Stage1.width;

		switch (key){
			case 'a':{
				switch (this->GameState->Board.arrayOrder){
					case 0:{
						if (layoutX - 1 >= 0){
							//if (layout[layoutY * width + (layoutX - 1)] == 0 && checkBlockCollision(this->GameState, x-1, y)){
							if (checkTile(layout[layoutY*width + (layoutX - 1)]) && checkBlockCollision(this->GameState, x - 1, y)){
								x--;
								this->GameState->PlayerState.x = x;
							}
						}
						break;
					}
					case 1:{
						if (layoutY + 1 < this->GameState->Stage1.height){
							//if (layout[(layoutY + 1)*width + layoutX] == 0 && checkBlockCollision(this->GameState, x, y+1)){
							if (checkTile(layout[(layoutY+1)*width + layoutX]) && checkBlockCollision(this->GameState, x, y + 1)){
								y++;
								this->GameState->PlayerState.y = y;
							}
						}
						break;
					}
					case 2:{
						if (layoutX + 1 < this->GameState->Stage1.width){
							//if (layout[layoutY * width + (layoutX + 1)] == 0 && checkBlockCollision(this->GameState, x+1, y)){
							if (checkTile(layout[layoutY*width + (layoutX + 1)]) && checkBlockCollision(this->GameState, x + 1, y)){
								x++;
								this->GameState->PlayerState.x = x;
							}
						}
						break;
					}
					case 3:{
						if (layoutY - 1 >= 0){
							//if (layout[(layoutY - 1)*width + layoutX] == 0 && checkBlockCollision(this->GameState, x, y-1)){
							if (checkTile(layout[(layoutY-1)*width+layoutX]) && checkBlockCollision(this->GameState, x, y - 1)){
								y--;
								this->GameState->PlayerState.y = y;
							}
						}
						break;
					}
				}
				if (!checkForExit(this->GameState, x, y)){
					this->setPosition(Position(x, y));
				}
				break;
			}
			case 'd':{
				switch (this->GameState->Board.arrayOrder){
					case 0:{
						if (layoutX + 1 < this->GameState->Stage1.width){
							//if (layout[layoutY * width + (layoutX + 1)] == 0 && checkBlockCollision(this->GameState, x+1, y)){
							if (checkTile(layout[layoutY*width + (layoutX + 1)]) && checkBlockCollision(this->GameState, x + 1, y)){
								x++;
								this->GameState->PlayerState.x = x;
							}
						}
						break;
					}
					case 1:{
						if (layoutY - 1 >= 0){
							//if (layout[(layoutY - 1)*width + layoutX] == 0 && checkBlockCollision(this->GameState, x, y-1)){
							if (checkTile(layout[(layoutY - 1)*width + layoutX]) && checkBlockCollision(this->GameState, x, y - 1)){
								y--;
								this->GameState->PlayerState.y = y;
							}
						}
						break;
					}
					case 2:{
						if (layoutX - 1 >= 0){
							//if (layout[layoutY*width + (layoutX - 1)] == 0 && checkBlockCollision(this->GameState, x - 1, y)){
							if (checkTile(layout[layoutY*width + (layoutX - 1)]) && checkBlockCollision(this->GameState, x - 1, y)){
								x--;
								this->GameState->PlayerState.x = x;
							}
						}
						break;
					}
					case 3:{
						if (layoutY + 1 < this->GameState->Stage1.height){
							//if (layout[(layoutY + 1)* width + layoutX] == 0 && checkBlockCollision(this->GameState, x, y+1)){
							if (checkTile(layout[(layoutY + 1)*width + layoutX]) && checkBlockCollision(this->GameState, x, y + 1)){
								y++;
								this->GameState->PlayerState.y = y;
							}
						}
						break;
					}
				}
				if (!checkForExit(this->GameState, x, y)){
					this->setPosition(Position(x, y));
				}
				break;
			}
			case 'q':{
				l.writeLog("This is Q. Rotates the arena counterclockwise. ");
				this->GameState->Board.rotateCCW = true;
				this->GameState->Board.rotateCW = false;
				this->GameState->Stage1.layout[this->GameState->PlayerState.y * this->GameState->Stage1.width + this->GameState->PlayerState.x] = 999;
				break;
			}
			case 'e': {
				l.writeLog("This is E. Rotates the arena clockwise. ");
				this->GameState->Board.rotateCW = true;
				this->GameState->Board.rotateCCW = false;
				this->GameState->Stage1.layout[this->GameState->PlayerState.y * this->GameState->Stage1.width + this->GameState->PlayerState.x] = 999;
				break;
			}
			default:{
				return 0;
			}
		}
	}
	else if (e->getType() == DF_STEP_EVENT){
		if (this->GameState->Board.isRotating){
			return 0;
		}
		if (this->GameState->win.win){
			return 0;
		}

		//Current location
		int x = this->getPosition().getX();
		int y = this->getPosition().getY();
		int layoutY = (y - this->GameState->Bounds.minY);
		int layoutX = (x - this->GameState->Bounds.minX);
		int* layout = this->GameState->Stage1.layout;

		if (this->GameState && this->GameState->Stage1.layout){
			//Gravity affected movement.
			//TODO(Thompson): Make sure when the player falls into the exit, it triggers
			//the next stage.
			switch (this->GameState->Board.arrayOrder){
				case 0:{
					if (layoutY + 1 < this->GameState->Stage1.height){
						if (checkTile(layout[(layoutY + 1)*this->GameState->Stage1.width + layoutX]) && checkBlockCollision(this->GameState, x, y+1)){
							y++;
						}
					}
					break;
				}
				case 1:{
					if (layoutX + 1 < this->GameState->Stage1.width){
						if (checkTile(layout[layoutY*this->GameState->Stage1.width + (layoutX + 1)]) && checkBlockCollision(this->GameState, x+1, y)){
							x++;
						}
					}
					break;
				}
				case 2:{
					if (layoutY - 1 >= 0){
						if (checkTile(layout[(layoutY - 1)*this->GameState->Stage1.width + layoutX]) && checkBlockCollision(this->GameState, x, y-1)){
							y--;
						}
					}
					break;
				}
				case 3:{
					if (layoutX - 1 >= 0){
						if (checkTile(layout[layoutY * this->GameState->Stage1.width + (layoutX - 1)]) && checkBlockCollision(this->GameState, x-1, y)){
							x--;
						}
					}
					break;
				}
			}
			checkForExit(this->GameState, x, y);
		}

		//Checks to see if the location is within bounds.
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

		//Set new position.
		this->GameState->PlayerState.x = x;
		this->GameState->PlayerState.y = y;
		this->setPosition(Position(x, y));
		return 1;
	}
	return 0;
}

void Player::draw(){
	if (this->GameState->Board.isRotating){
		return;
	}
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	int width = this->GameState->Stage1.width;
	int height = this->GameState->Stage1.height;
	switch (this->GameState->Board.arrayOrder){
		case 0:{
			Position pos = this->getPosition();
			pos.setXY(pos.getX() + 1, pos.getY()+1);
			g.drawCh(pos, '@', 0);
			break; 
		}
		case 1:{
			int x = this->GameState->PlayerState.x;
			int newY = this->GameState->Bounds.minY + (x - this->GameState->Bounds.minX);
			int y = this->GameState->PlayerState.y;
			int newX = this->GameState->Bounds.minX + ((this->GameState->Bounds.maxY-1) - y);
			Position newPos(newX + 1, newY + 1);
			g.drawCh(newPos, '@', 0);
			break;
		}
		case 2:{
			int x = this->GameState->PlayerState.x;
			int newX = (this->GameState->Bounds.maxX-1) - (x - this->GameState->Bounds.minX);
			int y = this->GameState->PlayerState.y;
			int newY = this->GameState->Bounds.minY + ((this->GameState->Bounds.maxY-1) - y);
			Position newPos(newX + 1, newY + 1);
			g.drawCh(newPos, '@', 0);
			break;
		}
		case 3:{
			int x = this->GameState->PlayerState.x;
			int newY = (this->GameState->Bounds.maxY-1) - (x - this->GameState->Bounds.minX);
			int y = this->GameState->PlayerState.y;
			int newX = (this->GameState->Bounds.maxX-1) - ((this->GameState->Bounds.maxY-1) - y);
			Position newPos(newX + 1, newY + 1);
			g.drawCh(newPos, '@', 0);
			break; 
		}
		default:{
			l.writeLog("[Player] Wrong array order: %d", (int)(this->GameState->Board.arrayOrder));
			break; 
		}
	}
	
	return;
}

void Player::initializeState(game_state* GameState){
	this->GameState = GameState;
	this->setPosition(Position(GameState->PlayerState.x, GameState->PlayerState.y));
	return;
}

