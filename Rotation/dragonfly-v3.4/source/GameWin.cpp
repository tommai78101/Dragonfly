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
		registerInterest(GAME_WIN_EVENT);

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
	unregisterInterest(GAME_WIN_EVENT);
}

int GameWin::eventHandler(Event* e){
	LogManager& l = LogManager::getInstance();
	WorldManager& w = WorldManager::getInstance();
	if (e->getType().compare(DF_KEYBOARD_EVENT) == 0){
		EventKeyboard* keyboard = dynamic_cast<EventKeyboard*>(e);
		if (keyboard->getKey() == ' '){
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