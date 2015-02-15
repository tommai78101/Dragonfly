#include "..\header\GameWin.h"
#include "..\header\Game.h"

GameWin::GameWin(game_state* GameState, Game* game){
	LogManager& l = LogManager::getInstance();
	ResourceManager& r = ResourceManager::getInstance();
	GraphicsManager& g = GraphicsManager::getInstance();

	Sprite* sprite = r.getSprite("game-win");
	if (sprite){
		setSprite(sprite);
		setTransparency();
		setSpriteSlowdown(5);

		int width = g.getHorizontal();
		int height = g.getVertical();

		Position pos(width / 2, height / 2);
		setPosition(pos);

		registerInterest(DF_KEYBOARD_EVENT);

		setVisible(true);
		this->GameState = GameState;
		this->game = game;
		l.writeLog("[GameWin] Successfully initialized winning text.");
		return;
	}
	l.writeLog("[GameWin] Something is wrong. Cannot initialize correctly.");
}

GameWin::~GameWin(){
	unregisterInterest(DF_KEYBOARD_EVENT);
}

int GameWin::eventHandler(Event* e){
	if (!this->GameState->win.win){
		return 0;
	}
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	if (e->getType().compare(DF_KEYBOARD_EVENT) == 0){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		if (keyboard->getKey() == ' ' || keyboard->getKey() == 'r'){
			w.markForDelete(this);
			this->game->reset();
			this->setVisible(false);
			this->GameState->win.isGameWinCreated = false;
			this->GameState->win.win = false;
			return 1;
		}
	}
	return 0;
}

void GameWin::draw(){
	Object::draw();
}