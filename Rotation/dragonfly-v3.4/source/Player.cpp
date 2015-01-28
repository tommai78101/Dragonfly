#include "..\header\Player.h"

Player::Player(game_state* GameState){
	LogManager& l = LogManager::getInstance();
	ResourceManager& r = ResourceManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	if (!l.isStarted() || !r.isStarted() || !w.isStarted()){
		l.writeLog("Manager was not started. Order by: %s %s %s", BoolToString(l.isStarted()), BoolToString(r.isStarted()), BoolToString(w.isStarted()));
		w.markForDelete(this);
		return;
	}
	
	//No need for a sprite. A simple character will do.
	setType(TYPE_PLAYER);
	setTransparency();
	setSolidness(Solidness::HARD);
	Position pos(GameState->PlayerState.x, GameState->PlayerState.y);
	setPosition(pos);

	registerInterest(DF_KEYBOARD_EVENT);
	registerInterest(DF_STEP_EVENT);

	this->GameState = GameState;
	setVisible(true);
	l.writeLog("[Player] Successfully loaded Player entity.");
}


//TODO: Add collision detection/response.
int Player::eventHandler(Event* e){
	LogManager& l = LogManager::getInstance();
	if (e->getType() == DF_KEYBOARD_EVENT){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		int key = keyboard->getKey();
		switch (key){
			case 'a':{
				int x = this->getPosition().getX();
				int layoutX = (x-2) - this->GameState->PlayerState.minX;
				if (layoutX >= 0){
					int layoutY = this->getPosition().getY() - this->GameState->PlayerState.minY -1;
					int width = this->GameState->Stage1.width;
					int* layout = this->GameState->Stage1.layout;
					if (layoutY*width + layoutX > 0){
						int check = *(layout + (layoutY *width + layoutX));
						if (check == 0){
							x--;
						}
					}
					else {
						l.writeLog("[Player] Error calculating X and Y, size limit is 286, actual value: %d", layoutY*width + layoutX);
					}
				}
				this->setPosition(Position(x, this->getPosition().getY()));
				break;
			}
			case 'd':{
				int x = this->getPosition().getX();
				int layoutX = (x) - this->GameState->PlayerState.minX;
				int width = this->GameState->Stage1.width;
				if (layoutX < width){
					int layoutY = this->getPosition().getY() - this->GameState->PlayerState.minY - 1;
					int* layout = this->GameState->Stage1.layout;
					if (layoutY*width+layoutX < 286){
						int check = *(layout + (layoutY *width + layoutX));
						if (check == 0){
							x++;
						}
					}
					else {
						l.writeLog("[Player] Error calculating X and Y, size limit is 286, actual value: %d", layoutY*width + layoutX);
					}
				}
				this->setPosition(Position(x, this->getPosition().getY()));
				break;
			}
			default:{
				return 0;
			}
		}
		return 1;
	}
	else if (e->getType() == DF_STEP_EVENT){
		//Current location
		int x = this->getPosition().getX();
		int y = this->getPosition().getY();

		if (this->GameState && this->GameState->Stage1.layout){
			int layoutY = (y - this->GameState->PlayerState.minY - 1);

			//Gravity affected movement.
			if (layoutY + 1 < this->GameState->Stage1.height){
				int layoutX = (x - this->GameState->PlayerState.minX);
				int width = this->GameState->Stage1.width;
				int* layout = this->GameState->Stage1.layout;
				int check = *(layout + ((layoutY+1)*width + layoutX));
				l.writeLog("Test: %d", check);
				if (check == 0 && check < 10 && check > -1){
					y++;
				}
			}
		}

		//Checks to see if the location is within bounds.
		if (x <= this->GameState->PlayerState.minX){
			x = this->GameState->PlayerState.minX + 1;
		}
		if (y <= this->GameState->PlayerState.minY){
			y = this->GameState->PlayerState.minY + 1;
		}
		if (x >= this->GameState->PlayerState.maxX){
			x = this->GameState->PlayerState.maxX - 1;
		}
		if (y > this->GameState->PlayerState.maxY){
			y = this->GameState->PlayerState.maxY;
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
	LogManager& l = LogManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();
	g.drawCh(this->getPosition(), '@', 0);
	return;
}

void Player::initializeState(game_state* GameState){
	this->GameState = GameState;
	this->setPosition(Position(GameState->PlayerState.x, GameState->PlayerState.y));
	return;
}

void Player::setGameBounds(int x, int y, int w, int h){
	if (this->GameState){
		this->GameState->PlayerState.minX = x;
		this->GameState->PlayerState.minY = y;
		this->GameState->PlayerState.maxX = w;
		this->GameState->PlayerState.maxY = h;
		LogManager& l = LogManager::getInstance();
		l.writeLog("setGameBounds = %d, %d, %d, %d", x, y, w, h);
	}
}